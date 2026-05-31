//source file
#include "../include/union_designated_initializer_multiple_fields_26.h"

// ========================================================================================================
#define MUT26_OUTPUT 1

void MutatorFrontendAction_26::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *UD = Result.Nodes.getNodeAs<clang::RecordDecl>("unions")) {
        if (!UD || !Result.Context->getSourceManager().isWrittenInMainFile(UD->getLocation()))
            return;

        std::string unionName = UD->getNameAsString();
        if (unionName.empty())
            return;

        if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("varDecl")) {
            if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(VD->getLocation()))
                return;

            std::string varInit = "{ .a = 10, .b = 20.5 }";
            std::string newInit = "/*mut26*/" + unionName + " " + VD->getNameAsString() + " = " + varInit + ";";
            Rewrite.ReplaceText(VD->getSourceRange(), newInit);
        }
    }
}

void MutatorFrontendAction_26::MutatorASTConsumer_26::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher unionMatcher = recordDecl(isUnion(), has(fieldDecl(hasType(builtinType()))), has(fieldDecl(hasType(namedDecl())))).bind("unions");
    DeclarationMatcher varMatcher = varDecl(hasType(recordDecl(hasName("UnionType")))).bind("varDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(unionMatcher, &callback);
    matchFinder.addMatcher(varMatcher, &callback);
    matchFinder.matchAST(Context);
}