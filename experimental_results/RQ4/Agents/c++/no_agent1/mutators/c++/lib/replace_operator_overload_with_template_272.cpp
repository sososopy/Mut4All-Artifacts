//source file
#include "../include/replace_operator_overload_with_template_272.h"

// ========================================================================================================
#define MUT272_OUTPUT 1

void MutatorFrontendAction_272::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("OperatorOverload")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (FD->isOverloadedOperator()) {
            auto declaration = stringutils::rangetoStr(*(Result.SourceManager), FD->getSourceRange());
            std::string templateDecl = "template<typename T> T operator" + FD->getNameAsString() + "(const T& lhs, const T& rhs) { return lhs " + FD->getNameAsString() + " rhs; }\n";
            declaration = "/*mut272*/" + templateDecl;

            Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), declaration);
        }
    }
}

void MutatorFrontendAction_272::MutatorASTConsumer_272::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isOverloadedOperator()).bind("OperatorOverload");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}