//source file
#include "../include/insert_unused_template_instantiation_49.h"

// ========================================================================================================
#define MUT49_OUTPUT 1

void MutatorFrontendAction_49::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                FD->getLocation()))
            return;

        if (!FD->isTemplateInstantiation()) {
            std::string funcName = FD->getNameAsString();
            if (instantiatedTemplates.find(funcName) == instantiatedTemplates.end()) {
                instantiatedTemplates.insert(funcName);
                std::string templateInst = "template<typename T> void " + funcName + "_unused() {}\n";
                templateInst += "/*mut49*/" + funcName + "_unused<int>();\n";
                Rewrite.InsertTextAfterToken(FD->getEndLoc(), templateInst);
            }
        }
    }
}

void MutatorFrontendAction_49::MutatorASTConsumer_49::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl().bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}