//source file
#include "../include/Mutator_Struct_Typedef_Conflict_421.h"

// ========================================================================================================
#define MUT421_OUTPUT 1

void MutatorFrontendAction_421::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::TypedefDecl>("Typedef")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;

        std::string typedefName = TD->getNameAsString();
        typedefNames.insert(typedefName);
    }
    else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        for (const auto &typedefName : typedefNames) {
            std::string structDecl = "struct " + typedefName + " { int x; };";
            Rewrite.InsertTextAfterToken(FD->getBody()->getBeginLoc(), "\n/*mut421*/" + structDecl + "\n");
        }
    }
}
  
void MutatorFrontendAction_421::MutatorASTConsumer_421::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher typedefMatcher = typedefDecl().bind("Typedef");
    DeclarationMatcher functionMatcher = functionDecl(hasBody(stmt())).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(typedefMatcher, &callback);
    matchFinder.addMatcher(functionMatcher, &callback);
    matchFinder.matchAST(Context);
}