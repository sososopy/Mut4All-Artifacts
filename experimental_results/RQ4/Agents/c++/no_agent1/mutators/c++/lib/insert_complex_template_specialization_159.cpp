//source file
#include "../include/insert_complex_template_specialization_159.h"

// ========================================================================================================
#define MUT159_OUTPUT 1

void MutatorFrontendAction_159::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;
        
        if (FD->hasBody()) {
            functions.push_back(FD);
        }
    } else if (auto *RD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Records")) {
        if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(RD->getLocation()))
            return;
        
        if (RD->isCompleteDefinition()) {
            std::string templateCode = "template<> struct Specialization<";
            templateCode += RD->getNameAsString() + "> { static const int value = 42; };";
            templateCode = "\n/*mut159*/" + templateCode;
            Rewrite.InsertTextAfterToken(RD->getEndLoc(), templateCode);
        }
    }
}

void MutatorFrontendAction_159::MutatorASTConsumer_159::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher functionMatcher = functionDecl().bind("Functions");
    DeclarationMatcher recordMatcher = cxxRecordDecl().bind("Records");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(functionMatcher, &callback);
    matchFinder.addMatcher(recordMatcher, &callback);
    matchFinder.matchAST(Context);
}