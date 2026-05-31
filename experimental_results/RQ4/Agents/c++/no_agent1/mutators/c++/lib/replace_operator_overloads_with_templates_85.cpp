//source file
#include "../include/replace_operator_overloads_with_templates_85.h"

// ========================================================================================================
#define MUT85_OUTPUT 1

void MutatorFrontendAction_85::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("OperatorOverload")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (FD->isOverloadedOperator() && FD->getNumParams() == 1) {
            auto paramType = FD->getParamDecl(0)->getType().getAsString();
            auto returnType = FD->getReturnType().getAsString();
            auto operatorName = FD->getNameAsString();

            std::string templateDecl = "template<typename T>\nT operator" + operatorName + "(const T& lhs, const T& rhs) {\n";
            templateDecl += "    return lhs " + operatorName + " rhs;\n}\n";

            SourceLocation startLoc = FD->getBeginLoc();
            SourceLocation endLoc = FD->getEndLoc().getLocWithOffset(1);

            Rewrite.ReplaceText(SourceRange(startLoc, endLoc), "/*mut85*/" + templateDecl);
        }
    }
}

void MutatorFrontendAction_85::MutatorASTConsumer_85::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isOverloadedOperator()).bind("OperatorOverload");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}