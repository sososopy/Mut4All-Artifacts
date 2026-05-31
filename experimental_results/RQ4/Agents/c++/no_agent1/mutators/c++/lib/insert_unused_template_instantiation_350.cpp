//source file
#include "../include/insert_unused_template_instantiation_350.h"

// ========================================================================================================
#define MUT350_OUTPUT 1

void MutatorFrontendAction_350::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *RD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("ClassDecl")) {
        if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(RD->getLocation()))
            return;

        if (processedClasses.find(RD) != processedClasses.end())
            return;
        
        processedClasses.insert(RD);

        if (!RD->isCompleteDefinition() || RD->isLambda())
            return;

        std::string className = RD->getNameAsString();
        if (className.empty())
            return;

        std::string templateCode = "\n/*mut350*/template<typename T> struct UnusedTemplate { void foo() {} }; UnusedTemplate<" + className + "> instance;\n";
        SourceLocation insertLoc = RD->getEndLoc();
        Rewrite.InsertTextAfterToken(insertLoc, templateCode);
    }
}

void MutatorFrontendAction_350::MutatorASTConsumer_350::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher classMatcher = cxxRecordDecl().bind("ClassDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(classMatcher, &callback);
    matchFinder.matchAST(Context);
}