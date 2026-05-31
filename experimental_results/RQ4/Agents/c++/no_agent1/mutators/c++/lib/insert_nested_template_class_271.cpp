//source file
#include "../include/insert_nested_template_class_271.h"

// ========================================================================================================
#define MUT271_OUTPUT 1

void MutatorFrontendAction_271::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *RD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("RecordDecl")) {
        if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(RD->getLocation()))
            return;
        
        if (!RD->isCompleteDefinition() || RD->isLambda())
            return;

        std::string nestedTemplate = R"(
        /*mut271*/template<typename T>
        class NestedTemplate {
        public:
            T value;
            NestedTemplate(T v) : value(v) {}
            T getValue() { return value; }
        };
        )";

        std::string insertionText = nestedTemplate;
        Rewrite.InsertTextAfterToken(RD->getEndLoc(), insertionText);
    }
}

void MutatorFrontendAction_271::MutatorASTConsumer_271::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl().bind("RecordDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}