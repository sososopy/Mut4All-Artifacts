//source file
#include "../include/template_instantiation_with_default_arguments_65.h"

// ========================================================================================================
#define MUT65_OUTPUT 1

void MutatorFrontendAction_65::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FuncTemplate")) {
        if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FTD->getLocation()))
            return;

        // Check if the template parameters already have default arguments
        bool hasDefaultArgs = false;
        for (auto *param : FTD->getTemplateParameters()->asArray()) {
            if (auto *tp = llvm::dyn_cast<clang::TemplateTypeParmDecl>(param)) {
                if (tp->hasDefaultArgument()) {
                    hasDefaultArgs = true;
                    break;
                }
            }
        }

        // If no default arguments exist, add a default argument to the first template parameter
        if (!hasDefaultArgs) {
            if (auto *tp = llvm::dyn_cast<clang::TemplateTypeParmDecl>(
                    FTD->getTemplateParameters()->getParam(0))) {
                std::string defaultArg = "int";
                std::string replacement = "typename " + tp->getNameAsString() + " = " + defaultArg;
                Rewrite.ReplaceText(tp->getSourceRange(), replacement);
            }
        }

        // Introduce a call to this function template with default arguments
        if (FTD->getTemplatedDecl()) {
            auto *FD = FTD->getTemplatedDecl();
            if (FD->hasBody()) {
                SourceLocation insertLoc = FD->getBody()->getEndLoc().getLocWithOffset(1);
                std::string call = "\n/*mut65*/" + FD->getNameAsString() + "();\n";
                Rewrite.InsertText(insertLoc, call);
            }
        }
    }
}

void MutatorFrontendAction_65::MutatorASTConsumer_65::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl().bind("FuncTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}