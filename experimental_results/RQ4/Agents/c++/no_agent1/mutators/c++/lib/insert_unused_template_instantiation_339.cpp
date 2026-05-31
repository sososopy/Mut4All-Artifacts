//source file
#include "../include/insert_unused_template_instantiation_339.h"

// ========================================================================================================
#define MUT339_OUTPUT 1

void MutatorFrontendAction_339::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FD->getLocation()))
            return;
        if (!FD->isThisDeclarationADefinition())
            return;

        functionDecls.push_back(FD);
    } else if (auto *CD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
        if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       CD->getLocation()))
            return;
        if (!CD->isCompleteDefinition())
            return;

        std::string className = CD->getNameAsString();
        if (className.empty())
            return;

        std::string templateCode = "template<typename T> class UnusedTemplate_" + className + " { public: void dummy() {} };";
        std::string instantiationCode = "UnusedTemplate_" + className + "<int> unusedInstance;";

        SourceLocation insertLoc = CD->getEndLoc();
        Rewrite.InsertTextAfterToken(insertLoc, "\n/*mut339*/" + templateCode + "\n" + instantiationCode + "\n");
    }
}

void MutatorFrontendAction_339::MutatorASTConsumer_339::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher functionMatcher = functionDecl().bind("Functions");
    DeclarationMatcher classMatcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(functionMatcher, &callback);
    matchFinder.addMatcher(classMatcher, &callback);
    matchFinder.matchAST(Context);
}