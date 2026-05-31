//source file
#include "../include/Mutate_Default_Capture_In_Nested_Generic_Lambda_23.h"

// ========================================================================================================
#define MUT23_OUTPUT 1

void MutatorFrontendAction_23::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("outerVar")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      //Record outer scope variables for potential use in capture replacement
      outerVars.push_back(VD);
    }
    else if (auto *L1 = Result.Nodes.getNodeAs<clang::LambdaExpr>("outerLambda")) {
      //Filter nodes in header files
      if (!L1 || !Result.Context->getSourceManager().isWrittenInMainFile(
                     L1->getBeginLoc()))
        return;
      
      // Check if L1 has a parameter with default argument that is a lambda
      auto *CallOp = L1->getCallOperator();
      if (!CallOp || CallOp->param_empty()) return;
      
      // Look for parameters with default arguments that are lambda expressions
      for (unsigned i = 0; i < CallOp->getNumParams(); ++i) {
        auto *PVD = CallOp->getParamDecl(i);
        if (PVD->hasDefaultArg()) {
          auto *DefaultArg = PVD->getDefaultArg();
          if (DefaultArg && isa<clang::LambdaExpr>(DefaultArg)) {
            auto *L2 = cast<clang::LambdaExpr>(DefaultArg);
            // Check if L2 has a capture with default initializer
            auto Captures = L2->captures();
            for (auto &Cap : Captures) {
              if (Cap.capturesVariable() && Cap.isExplicit() && Cap.getCaptureKind() == clang::LCK_ByCopy && Cap.getCapturedVar()->hasInit()) {
                // Found target structure: L1 -> param with default L2 -> capture with init
                targetLambdas.push_back(L1);
                break;
              }
            }
          }
        }
      }
    }
    else if (auto *L1 = Result.Nodes.getNodeAs<clang::LambdaExpr>("processLambda")) {
      //Filter nodes in header files
      if (!L1 || !Result.Context->getSourceManager().isWrittenInMainFile(
                     L1->getBeginLoc()))
        return;
      
      // Process only if this lambda was identified as a target earlier
      if (std::find(targetLambdas.begin(), targetLambdas.end(), L1) == targetLambdas.end())
        return;
        
      auto *CallOp = L1->getCallOperator();
      if (!CallOp) return;
      
      // Find the parameter with lambda default argument
      clang::LambdaExpr* L2 = nullptr;
      clang::ParmVarDecl* TargetParam = nullptr;
      const clang::LambdaCapture* TargetCapture = nullptr;
      const clang::VarDecl* CapturedVar = nullptr;
      
      for (unsigned i = 0; i < CallOp->getNumParams(); ++i) {
        auto *PVD = CallOp->getParamDecl(i);
        if (PVD->hasDefaultArg()) {
          auto *DefaultArg = PVD->getDefaultArg();
          if (DefaultArg && isa<clang::LambdaExpr>(DefaultArg)) {
            L2 = cast<clang::LambdaExpr>(DefaultArg);
            auto Captures = L2->captures();
            for (auto &Cap : Captures) {
              if (Cap.capturesVariable() && Cap.isExplicit() && Cap.getCaptureKind() == clang::LCK_ByCopy && Cap.getCapturedVar()->hasInit()) {
                TargetParam = PVD;
                TargetCapture = &Cap;
                CapturedVar = Cap.getCapturedVar();
                break;
              }
            }
            if (TargetCapture) break;
          }
        }
      }
      
      if (!L2 || !TargetParam || !TargetCapture || !CapturedVar) return;
      
      //Get the source code text of target node
      SourceManager &SM = Rewrite.getSourceMgr();
      LangOptions LangOpts;
      
      // Get the captured variable's initializer source range
      auto InitRange = CapturedVar->getInit()->getSourceRange();
      if (InitRange.isInvalid()) return;
      
      std::string InitText = clang::Lexer::getSourceText(
          clang::CharSourceRange::getTokenRange(InitRange), SM, LangOpts).str();
      
      // Look for a suitable replacement variable
      std::string ReplacementVar;
      bool NeedNewVar = true;
      
      // Try to find an existing variable in outer scope
      for (auto *VD : outerVars) {
        if (VD->getType()->isIntegerType() || VD->getType()->isFloatingType()) {
          // Check if variable is in scope at L1's definition
          SourceLocation L1Loc = L1->getBeginLoc();
          SourceLocation VDLoc = VD->getBeginLoc();
          
          if (SM.isBeforeInTranslationUnit(VDLoc, L1Loc)) {
            ReplacementVar = VD->getNameAsString();
            NeedNewVar = false;
            break;
          }
        }
      }
      
      // If no suitable variable found, create a new one
      if (NeedNewVar) {
        // Choose a simple type and constant value
        std::string NewVarName = "mut23_var";
        std::string NewVarType = "int";
        std::string NewVarValue = "3";
        
        // Insert new variable declaration before L1
        std::string NewVarDecl = NewVarType + " " + NewVarName + " = " + NewVarValue + ";\n";
        SourceLocation InsertLoc = L1->getBeginLoc();
        
        // Check if we're at start of line to avoid breaking existing code
        if (InsertLoc.isMacroID()) {
          InsertLoc = SM.getExpansionLoc(InsertLoc);
        }
        
        // Insert the new variable declaration
        Rewrite.InsertTextBefore(InsertLoc, "/*mut23*/" + NewVarDecl);
        ReplacementVar = NewVarName;
      }
      
      //Perform mutation on the source code text by applying string replacement
      std::string NewInitText = ReplacementVar;
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(InitRange, NewInitText);
    }
}
  
void MutatorFrontendAction_23::MutatorASTConsumer_23::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    // First matcher to collect outer scope variables
    DeclarationMatcher varMatcher = varDecl(unless(parmVarDecl())).bind("outerVar");
    // Second matcher to find outer lambdas with generic parameters
    StatementMatcher lambdaMatcher = lambdaExpr().bind("outerLambda");
    // Third matcher to process the identified lambdas
    StatementMatcher processMatcher = lambdaExpr().bind("processLambda");
    
    Callback callback(TheRewriter);
    matchFinder.addMatcher(varMatcher, &callback);
    matchFinder.addMatcher(lambdaMatcher, &callback);
    matchFinder.addMatcher(processMatcher, &callback);
    matchFinder.matchAST(Context);
}