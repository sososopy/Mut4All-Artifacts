//source file
#include "../include/insert_recursive_template_function_229.h"

// ========================================================================================================
#define MUT229_OUTPUT 1

void MutatorFrontendAction_229::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FD->getLocation()))
            return;

        if (FD->hasBody() && !FD->isTemplateInstantiation()) {
            auto functionName = FD->getNameAsString();
            auto returnType = FD->getReturnType().getAsString();

            std::string templateFunction = "template<int N>\n";
            templateFunction += returnType + " " + functionName + "_recursive() {\n";
            templateFunction += "if constexpr (N > 0) return " + functionName + "_recursive<N-1>();\n";
            templateFunction += "return " + functionName + "();\n";
            templateFunction += "}\n";

            Rewrite.InsertTextBefore(FD->getBeginLoc(), "/*mut229*/" + templateFunction);
        }
    }
}

void MutatorFrontendAction_229::MutatorASTConsumer_229::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isDefinition()).bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}