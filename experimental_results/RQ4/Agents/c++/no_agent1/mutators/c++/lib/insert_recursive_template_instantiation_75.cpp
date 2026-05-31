//source file
#include "../include/insert_recursive_template_instantiation_75.h"

// ========================================================================================================
#define MUT75_OUTPUT 1

void MutatorFrontendAction_75::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
            return;
        
        if (FD->hasBody() && FD->isTemplateInstantiation()) {
            recursiveFunctions.push_back(FD);
        }
    } else if (auto *RT = Result.Nodes.getNodeAs<clang::ReturnStmt>("ReturnStmt")) {
        if (!RT || !Result.Context->getSourceManager().isWrittenInMainFile(
                       RT->getBeginLoc()))
            return;

        if (!recursiveFunctions.empty()) {
            size_t index = getrandom::getRandomIndex(recursiveFunctions.size() - 1);
            auto targetFunc = recursiveFunctions[index];

            std::string funcName = targetFunc->getNameAsString();
            std::string templateCall = "/*mut75*/" + funcName + "<int>();";

            Rewrite.InsertTextAfterToken(RT->getEndLoc(), templateCall);
        }
    }
}

void MutatorFrontendAction_75::MutatorASTConsumer_75::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto functionMatcher = functionDecl(isTemplateInstantiation()).bind("Function");
    auto returnStmtMatcher = returnStmt().bind("ReturnStmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(functionMatcher, &callback);
    matchFinder.addMatcher(returnStmtMatcher, &callback);
    matchFinder.matchAST(Context);
}