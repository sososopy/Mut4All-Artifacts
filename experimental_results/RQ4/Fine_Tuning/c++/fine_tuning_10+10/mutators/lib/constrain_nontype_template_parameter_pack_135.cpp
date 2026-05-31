//source file
#include "../include/Constrain_NonType_Template_Parameter_Pack_13535.h"

// ========================================================================================================
#define MUT135_OUTPUT 1

void MutatorFrontendAction_13535::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      auto &SM = Result.SourceManager;
      auto &LangOpts = Result.Context->getLangOpts();
      auto &TheRewriter = Rewrite;
      
      auto TemplateParams = FD->getTemplateParameters();
      if (TemplateParams->size() < 1)
        return;

      // Create a new concept definition
      std::string conceptDef = "\n/*mut135*/template<typename T> concept ConceptConstraint = true;\n";
      TheRewriter.InsertTextBefore(FD->getBeginLoc(), conceptDef);

      // Apply requires clause to the function template
      std::string requiresClause = "requires (ConceptConstraint<decltype(values)> && ...)";
      auto FuncDecl = FD->getTemplatedDecl();
      auto FuncLoc = FuncDecl->getBeginLoc();
      TheRewriter.InsertTextAfterToken(FuncLoc, requiresClause);
    }
}

void MutatorFrontendAction_13535::MutatorASTConsumer_13535::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl(hasDescendant(templateTypeParmDecl())).bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}