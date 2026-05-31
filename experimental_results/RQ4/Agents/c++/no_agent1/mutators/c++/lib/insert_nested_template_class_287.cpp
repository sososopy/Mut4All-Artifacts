//source file
#include "../include/insert_nested_template_class_287.h"

// ========================================================================================================
#define MUT287_OUTPUT 1

void MutatorFrontendAction_287::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *RD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("RecordDecl")) {
        //Filter nodes in header files
        if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(RD->getLocation()))
            return;
        if (!RD->isCompleteDefinition() || RD->isLambda())
            return;

        std::string className = RD->getNameAsString();
        if (visitedClasses.find(className) != visitedClasses.end())
            return;

        visitedClasses.insert(className);

        //Get the source code text of target node
        std::string nestedTemplate = "\n/*mut287*/template<typename T> class NestedTemplate { public: T value; };";

        //Perform mutation on the source code text by applying string replacement
        std::string insertion = nestedTemplate + "\n" + className + "::NestedTemplate<int> nestedInstance;";

        //Replace the original AST node with the mutated one
        Rewrite.InsertTextAfterToken(RD->getEndLoc(), insertion);
    }
}

void MutatorFrontendAction_287::MutatorASTConsumer_287::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = cxxRecordDecl().bind("RecordDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}