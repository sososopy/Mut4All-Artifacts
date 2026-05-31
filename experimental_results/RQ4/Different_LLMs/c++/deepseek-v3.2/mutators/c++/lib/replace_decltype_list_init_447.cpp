//source file
#include "../include/Replace_Decltype_List_Init_447.h"

// ========================================================================================================
#define MUT447_OUTPUT 1

void MutatorFrontendAction_447::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("varDecl")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      //Check if initialization uses decltype
      const Expr* InitExpr = VD->getInit();
      if (!InitExpr) return;
      //Check if it's a CXXConstructExpr with list initialization
      if (const CXXConstructExpr* CCE = dyn_cast<CXXConstructExpr>(InitExpr)) {
        if (CCE->getNumArgs() != 1) return;
        const Expr* Arg = CCE->getArg(0);
        if (!Arg) return;
        //Check if the argument is an InitListExpr with empty list
        if (const InitListExpr* ILE = dyn_cast<InitListExpr>(Arg)) {
          if (ILE->getNumInits() != 0) return;
          //Check if the type is decltype
          const TypeSourceInfo* TSI = VD->getTypeSourceInfo();
          if (!TSI) return;
          const Type* T = TSI->getType().getTypePtr();
          if (!T) return;
          //Check if type is DecltypeType
          if (const DecltypeType* DT = dyn_cast<DecltypeType>(T)) {
            //Check if underlying expression is a function call or reference type
            const Expr* UnderlyingExpr = DT->getUnderlyingExpr();
            if (!UnderlyingExpr) return;
            //Check if it yields a reference type
            if (!DT->getUnderlyingType()->isReferenceType()) return;
            //Get the source code text of target node
            SourceRange VarRange = VD->getSourceRange();
            std::string OriginalText = stringutils::rangetoStr(*(Result.SourceManager), VarRange);
            //Find the position of {} and replace with ()
            size_t BracePos = OriginalText.find("{}");
            if (BracePos == std::string::npos) return;
            std::string MutatedText = OriginalText;
            MutatedText.replace(BracePos, 2, "()");
            //Perform mutation on the source code text by applying string replacement
            //Replace the original AST node with the mutated one
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(VarRange), MutatedText);
          }
        }
      }
    }
}
  
void MutatorFrontendAction_447::MutatorASTConsumer_447::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(hasInitializer(cxxConstructExpr(hasArgument(0, initListExpr(hasInit(0, expr())))))).bind("varDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}