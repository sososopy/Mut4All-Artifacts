//source file
#include "../include/insert_recursive_template_instantiation_446.h"

// ========================================================================================================
#define MUT446_OUTPUT 1

void MutatorFrontendAction_446::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *RD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("RecursiveClass")) {
        if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(RD->getLocation()))
            return;
        
        if (processedClasses.find(RD) != processedClasses.end())
            return;

        processedClasses.insert(RD);

        if (!RD->isCompleteDefinition())
            return;

        std::string className = RD->getNameAsString();
        std::string templateDecl = "template<int N> struct " + className + " {\n"
                                   "  " + className + "<N-1> member;\n"
                                   "};\n"
                                   "template<> struct " + className + "<0> {};\n";
        
        std::string insertionText = "/*mut446*/" + templateDecl;
        Rewrite.InsertTextBefore(RD->getBeginLoc(), insertionText);
    }
}

void MutatorFrontendAction_446::MutatorASTConsumer_446::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(isDefinition(), unless(isLambda())).bind("RecursiveClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}