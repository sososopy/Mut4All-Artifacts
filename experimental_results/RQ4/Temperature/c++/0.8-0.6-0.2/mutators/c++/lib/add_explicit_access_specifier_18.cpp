//source file
#include "../include/add_explicit_access_specifier_18.h"

// ========================================================================================================
#define MUT18_OUTPUT 1

void MutatorFrontendAction_18::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *RD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("CXXRecord")) {
        if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(RD->getLocation()))
            return;

        if (!RD->isCompleteDefinition())
            return;

        const bool isStruct = RD->isStruct();
        const char *defaultSpecifier = isStruct ? "public" : "private";

        auto beginLoc = RD->getBraceRange().getBegin();
        std::string explicitSpecifier = std::string(defaultSpecifier) + ":\n";

        Rewrite.InsertTextAfterToken(beginLoc, "/*mut18*/\n" + explicitSpecifier);

        for (const auto *D : RD->decls()) {
            if (D->isImplicit() || D->getAccess() != clang::AS_none)
                continue;

            clang::AccessSpecifier explicitAccess = isStruct ? clang::AS_public : clang::AS_private;
            std::string memberSpecifier = (explicitAccess == clang::AS_public ? "public" : "private");

            auto declBeginLoc = D->getBeginLoc();
            Rewrite.InsertTextBefore(declBeginLoc, memberSpecifier + ":\n");
        }
    }
}
  
void MutatorFrontendAction_18::MutatorASTConsumer_18::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl().bind("CXXRecord");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}