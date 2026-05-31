//source file
#include "../include/insert_nested_template_class_243.h"

// ========================================================================================================
#define MUT243_OUTPUT 1

void MutatorFrontendAction_243::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *RD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("RecordDecl")) {
        if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(
                        RD->getLocation()))
            return;
        if (!RD->isCompleteDefinition())
            return;

        // Get the source code text of target node
        auto className = RD->getNameAsString();
        if (className.empty())
            return;

        // Perform mutation by inserting a nested template class
        std::string templateClass = "\n/*mut243*/template<typename T>\nclass NestedTemplate {\npublic:\n    T value;\n    NestedTemplate(T v) : value(v) {}\n    void display() { /* some logic */ }\n};\n";
        
        // Insert the nested template class definition
        Rewrite.InsertTextAfterToken(RD->getEndLoc(), templateClass);
    }
}

void MutatorFrontendAction_243::MutatorASTConsumer_243::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl(isDefinition()).bind("RecordDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}