//source file
#include "../include/insert_nested_template_class_181.h"

// ========================================================================================================
#define MUT181_OUTPUT 1

void MutatorFrontendAction_181::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *RD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Record")) {
        //Filter nodes in header files
        if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       RD->getLocation()))
            return;
        if (!RD->isCompleteDefinition())
            return;
        
        //Get the source code text of target node
        auto className = RD->getNameAsString();
        if (className.empty())
            return;

        //Perform mutation on the source code text by applying string replacement
        std::string nestedTemplate = "\n/*mut181*/template<typename T>\nclass NestedTemplate {\npublic:\n    T value;\n    NestedTemplate(T v) : value(v) {}\n};\n";
        std::string insertion = "public:\n" + nestedTemplate;
        
        //Replace the original AST node with the mutated one
        Rewrite.InsertTextAfterToken(RD->getEndLoc().getLocWithOffset(-1), insertion);
    }
}

void MutatorFrontendAction_181::MutatorASTConsumer_181::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = cxxRecordDecl().bind("Record");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}