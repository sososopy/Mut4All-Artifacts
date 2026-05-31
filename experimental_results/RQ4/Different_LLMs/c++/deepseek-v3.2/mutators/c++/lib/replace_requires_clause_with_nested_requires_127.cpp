//source file
#include "../include/Replace_Requires_Clause_With_Nested_Requires_127.h"

// ========================================================================================================
#define MUT127_OUTPUT 1

void MutatorFrontendAction_127::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Check if it's a templated function with a requires clause
      if (!FD->getDescribedFunctionTemplate()) return;
      const FunctionTemplateDecl *FTD = FD->getDescribedFunctionTemplate();
      if (!FTD) return;
      
      //Check for template parameter pack
      bool hasPack = false;
      for (unsigned i = 0; i < FTD->getTemplateParameters()->size(); ++i) {
        if (isa<TemplateTypeParmDecl>(FTD->getTemplateParameters()->getParam(i))) {
          auto *TTP = cast<TemplateTypeParmDecl>(FTD->getTemplateParameters()->getParam(i));
          if (TTP->isParameterPack()) {
            hasPack = true;
            break;
          }
        } else if (isa<NonTypeTemplateParmDecl>(FTD->getTemplateParameters()->getParam(i))) {
          auto *NTTP = cast<NonTypeTemplateParmDecl>(FTD->getTemplateParameters()->getParam(i));
          if (NTTP->isParameterPack()) {
            hasPack = true;
            break;
          }
        } else if (isa<TemplateTemplateParmDecl>(FTD->getTemplateParameters()->getParam(i))) {
          auto *TTP = cast<TemplateTemplateParmDecl>(FTD->getTemplateParameters()->getParam(i));
          if (TTP->isParameterPack()) {
            hasPack = true;
            break;
          }
        }
      }
      if (!hasPack) return;
      
      //Check for requires clause
      const Expr *RequiresExpr = FD->getTrailingRequiresClause();
      if (!RequiresExpr) return;
      
      //Get the source code text of target node
      SourceLocation StartLoc = FD->getBeginLoc();
      SourceLocation EndLoc = FD->getEndLoc();
      if (StartLoc.isInvalid() || EndLoc.isInvalid()) return;
      
      SourceManager &SM = Rewrite.getSourceMgr();
      
      //Get the full function declaration text
      std::string FuncText = stringutils::rangetoStr(SM, SourceRange(StartLoc, EndLoc));
      
      //Find the requires clause in the text
      std::string RequiresStr = stringutils::rangetoStr(SM, RequiresExpr->getSourceRange());
      if (RequiresStr.empty()) return;
      
      //Check if the requires clause contains parameter pack expansion
      //We'll look for patterns like "args..." or "..."
      if (RequiresStr.find("...") == std::string::npos) return;
      
      //Perform mutation: replace simple requires clause with nested requires expression
      //Original: requires(T(args...))
      //Mutated: requires(requires{ T(args...); })
      std::string MutatedRequires = "requires(requires{ " + RequiresStr.substr(10) + "; })";
      
      //Replace the requires clause in the function text
      size_t Pos = FuncText.find(RequiresStr);
      if (Pos == std::string::npos) return;
      
      std::string MutatedFuncText = FuncText;
      MutatedFuncText.replace(Pos, RequiresStr.length(), MutatedRequires);
      MutatedFuncText = "/*mut127*/" + MutatedFuncText;
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), MutatedFuncText);
    }
}
  
void MutatorFrontendAction_127::MutatorASTConsumer_127::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(hasTrailingReturn(), unless(isImplicit())).bind("FunctionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}