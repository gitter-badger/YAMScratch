function compCopy(op, np)
%COMPCOPY copies a figure object represented by "op" and its % descendants to another figure "np" preserving the same hierarchy.
% stright from <http://www.mathworks.com/matlabcentral/answers/102714-how-do-i-copy-a-figure-object-and-its-descendants-to-another-object-of-the-same-class-in-matlab>
ch = get(op, 'children');
    if ~isempty(ch)   
        nh = copyobj(ch,np);
        for k = 1:length(ch)
            compCopy(ch(k),nh(k));
        end
    end;
    return;
end