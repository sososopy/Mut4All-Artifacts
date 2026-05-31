//source file
#include "../include/insert_nested_template_class_451.h"

// ========================================================================================================
#define MUT451_OUTPUT 1

void MutatorFrontendAction_451::Callback::run(const MatchFinder::MatchResult &Result) {
    // Check whether the matched AST node is the target node
    if (auto *RD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("RecordDecl")) {
        // Filter nodes in header files
        if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(RD->getLocation()))
            return;
        if (!RD->isCompleteDefinition() || RD->isLambda())
            return;

        // Get the source code text of target node
        auto recordName = RD->getNameAsString();
        if (recordName.empty())
            return;

        // Perform mutation on the source code text by applying string replacement
        std::string nestedTemplate = "\n/*mut451*/template<typename T> class NestedTemplate { "
                                     "public: T value; NestedTemplate(T v) : value(v) {} };";
        std::string insertion = nestedTemplate + "\n";
        
        // Replace the original AST node with the mutated one
        Rewrite.InsertTextAfter(RD->getEndLoc().getLocWithOffset(-1), insertion);
    }
}

void MutatorFrontendAction_451::MutatorASTConsumer_451::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = cxxRecordDecl(isDefinition()).bind("RecordDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}