//source file
#include "../include/insert_recursive_template_function_146.h"

// ========================================================================================================
#define MUT146_OUTPUT 1

void MutatorFrontendAction_146::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
            return;

        if (FD->hasBody()) {
            auto functionName = FD->getNameAsString();
            auto returnType = FD->getReturnType().getAsString();
            std::string templateFunction = "template<int N>\n" + returnType + " " + functionName + "_template() {\n";
            templateFunction += "if constexpr (N > 0) {\nreturn " + functionName + "_template<N-1>();\n} else {\nreturn " + functionName + "();\n}\n}\n";

            Rewrite.InsertTextBefore(FD->getBeginLoc(), "/*mut146*/" + templateFunction);
        }
    }
}
  
void MutatorFrontendAction_146::MutatorASTConsumer_146::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isDefinition(), unless(isTemplateInstantiation())).bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}