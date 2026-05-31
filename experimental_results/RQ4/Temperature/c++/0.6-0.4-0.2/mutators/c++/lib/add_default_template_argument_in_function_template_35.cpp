//source file
#include "../include/add_default_template_argument_in_function_template_35.h"

// ========================================================================================================
#define MUT35_OUTPUT 1

void MutatorFrontendAction_35::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
        if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(FTD->getLocation()))
            return;

        if (FTD->getTemplateParameters()->size() > 0) {
            auto *param = FTD->getTemplateParameters()->getParam(0);
            if (auto *typeParam = llvm::dyn_cast<clang::TemplateTypeParmDecl>(param)) {
                if (!typeParam->hasDefaultArgument()) {
                    std::string defaultArg = "int";
                    std::string mutation = typeParam->getNameAsString() + " = " + defaultArg;
                    SourceLocation insertLoc = typeParam->getEndLoc().getLocWithOffset(1);
                    Rewrite.InsertText(insertLoc, " = " + defaultArg, true, true);
                }
            }
        }
    }
}
  
void MutatorFrontendAction_35::MutatorASTConsumer_35::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl().bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}