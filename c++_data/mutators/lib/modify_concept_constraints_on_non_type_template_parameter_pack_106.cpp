//source file
#include "../include/Modify_Concept_Constraints_On_Non_Type_Template_Parameter_Pack_106.h"

// ========================================================================================================
#define MUT106_OUTPUT 1

void MutatorFrontendAction_106::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("MemberTemplate")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;

      auto &SM = *Result.SourceManager;
      auto &LangOpts = Result.Context->getLangOpts();
      auto TemplateParams = MT->getTemplateParameters();
      
      std::string NewTemplateParamStr;
      std::string RequiresClauseStr;
      
      for (auto *Param : *TemplateParams) {
        if (auto *NTTP = llvm::dyn_cast<clang::NonTypeTemplateParmDecl>(Param)) {
          if (NTTP->getType()->isDependentType()) {
            NewTemplateParamStr += "auto... " + NTTP->getNameAsString();
            RequiresClauseStr += "requires (... && ConceptName<decltype(xs)>)";
          } else {
            NewTemplateParamStr += NTTP->getType().getAsString() + " " + NTTP->getNameAsString();
          }
        } else {
          NewTemplateParamStr += Param->getSourceRange().printToString(SM);
        }
        NewTemplateParamStr += ", ";
      }
      
      if (!NewTemplateParamStr.empty())
        NewTemplateParamStr.pop_back();
      
      auto TemplateRange = clang::CharSourceRange::getTokenRange(MT->getSourceRange());
      std::string OriginalTemplateText = clang::Lexer::getSourceText(TemplateRange, SM, LangOpts).str();
      
      size_t TemplateStart = OriginalTemplateText.find("template<");
      size_t TemplateEnd = OriginalTemplateText.find('>', TemplateStart);
      
      std::string NewTemplateText = "template<" + NewTemplateParamStr + ">";
      if (!RequiresClauseStr.empty()) {
        NewTemplateText += " " + RequiresClauseStr;
      }
      
      OriginalTemplateText.replace(TemplateStart, TemplateEnd - TemplateStart + 1, NewTemplateText);
      
      Rewrite.ReplaceText(TemplateRange, OriginalTemplateText);
    }
}
  
void MutatorFrontendAction_106::MutatorASTConsumer_106::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl(
        hasDescendant(nonTypeTemplateParmDecl())
    ).bind("MemberTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}