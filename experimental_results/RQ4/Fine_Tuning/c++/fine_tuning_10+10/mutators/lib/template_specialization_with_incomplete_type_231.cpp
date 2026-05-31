//source file
#include "../include/template_specialization_with_incomplete_type_231.h"

// ========================================================================================================
#define MUT231_OUTPUT 1

void MutatorFrontendAction_231::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("ClassTemplate")) {
        if (!CT || !Result.Context->getSourceManager().isWrittenInMainFile(
                       CT->getLocation()))
            return;

        // Insert incomplete type declaration
        std::string incompleteType = "\n/*mut231*/enum struct UndefinedEnum;";
        Rewrite.InsertTextBefore(CT->getBeginLoc(), incompleteType);

        // Insert template specialization with incomplete type
        std::string specialization = "\n/*mut231*/template <class T> struct CheckType : decltype(test<T>(0)) {};";
        Rewrite.InsertTextAfterToken(CT->getEndLoc(), specialization);

        // Insert static assertion
        std::string staticAssertion = "\n/*mut231*/static_assert(CheckType<UndefinedEnum>::value, \"Type check failed.\");";
        Rewrite.InsertTextAfterToken(CT->getEndLoc(), staticAssertion);
    }
}

void MutatorFrontendAction_231::MutatorASTConsumer_231::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl().bind("ClassTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}