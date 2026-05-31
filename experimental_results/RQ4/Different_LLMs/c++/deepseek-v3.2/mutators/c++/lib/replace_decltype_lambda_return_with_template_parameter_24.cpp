//source file
#include "../include/Mutator_Replace_Decltype_Lambda_Return_With_Template_Parameter_24.h"

// ========================================================================================================
#define MUT24_OUTPUT 1

void MutatorFrontendAction_24::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambdaExpr")) {
      //Filter nodes in header files
      if (!LE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LE->getBeginLoc()))
        return;
      
      // Get the lambda's call operator
      auto *Callee = LE->getCallOperator();
      if (!Callee) return;
      
      // Check if lambda is inside a function template
      auto *FD = dyn_cast<FunctionDecl>(Callee->getParent());
      if (!FD) return;
      auto *FT = FD->getTemplateSpecializationArgs();
      if (!FT) return;
      
      // Get the return type of the lambda
      auto ReturnType = Callee->getReturnType();
      if (!ReturnType.getTypePtr()) return;
      
      // Check if return type is a decltype
      auto *DT = dyn_cast<DecltypeType>(ReturnType.getTypePtr());
      if (!DT) return;
      
      // Get the underlying expression of the decltype
      auto *E = DT->getUnderlyingExpr();
      if (!E) return;
      
      // Check if the expression references a captured variable
      auto *DRE = dyn_cast<DeclRefExpr>(E);
      if (!DRE) return;
      
      auto *VD = dyn_cast<VarDecl>(DRE->getDecl());
      if (!VD) return;
      
      // Check if the variable is a parameter of the enclosing function template
      bool IsParam = false;
      for (auto *P : FD->parameters()) {
        if (P == VD) {
          IsParam = true;
          break;
        }
      }
      if (!IsParam) return;
      
      // Get the template parameter type
      if (FT->size() == 0)  return;
      auto TemplateParamType = FT->get(0).getAsType();
      if (TemplateParamType.isNull()) return;
      
      // Get the source range of the decltype expression
      SourceRange DecltypeRange;
      auto LambdaRange = LE->getSourceRange();
      auto LambdaText = stringutils::rangetoStr(*(Result.SourceManager), LambdaRange);
      size_t Pos = LambdaText.find("decltype");
      if (Pos == std::string::npos) return;
      
      // Find the end of the decltype expression
      size_t End = Pos;
      int ParenCount = 0;
      for (; End < LambdaText.size(); ++End) {
        if (LambdaText[End] == '(') ++ParenCount;
        else if (LambdaText[End] == ')') {
          --ParenCount;
          if (ParenCount == 0) {
            ++End;
            break;
          }
        }
      }
      DecltypeRange = SourceRange(LambdaRange.getBegin().getLocWithOffset(Pos),
                                  LambdaRange.getBegin().getLocWithOffset(End));
      
      // Get the template parameter type as a string
      std::string TypeStr;
      llvm::raw_string_ostream OS(TypeStr);
      TemplateParamType.print(OS, Result.Context->getPrintingPolicy());
      
      // Perform mutation: replace decltype(...) with template parameter type
      std::string Replacement = "/*mut24*/" + OS.str();
      
      // Replace the original AST node with the mutated one
      Rewrite.ReplaceText(DecltypeRange, Replacement);
    }
}
  
void MutatorFrontendAction_24::MutatorASTConsumer_24::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = lambdaExpr().bind("lambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}