def iter_case(I):
    if not I:
        yield ""
    else:
        F = I[:1]
        if F.lower() == F.upper():
            for S in iter_case(I[1:]):
                yield F + S
        else:
            for S in iter_case(I[1:]):
                yield F.lower() + S
                yield F.upper() + S


for y in iter_case("THere was once an man from numtucet. %(^&%~.,"):
    print y