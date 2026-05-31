//source file
#include "../include/Mutator_12.h"

// ========================================================================================================
#define MUT12_OUTPUT 1

void MutatorFrontendAction_12::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FieldDecl>("FieldDecl")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      
      // Check if the field has a default member initializer with a lambda
      if (!FD->hasInClassInitializer())
        return;
      
      const Expr *Init = FD->getInClassInitializer();
      if (!Init)
        return;
      
      // Check if the initializer is a lambda expression
      if (auto *LE = dyn_cast<LambdaExpr>(Init)) {
        //Get the source code text of target node
        SourceRange LambdaRange = LE->getSourceRange();
        std::string LambdaText = stringutils::rangetoStr(*(Result.SourceManager), LambdaRange);
        
        // Extract lambda body and parameters
        std::string LambdaBody = stringutils::rangetoStr(*(Result.SourceManager), LE->getBody()->getSourceRange());
        std::string ParamsText;
        if (LE->getLambdaClass()->getLambdaCallOperator()) {
          auto *CallOp = LE->getLambdaClass()->getLambdaCallOperator();
          if (CallOp->getNumParams() > 0) {
            ParamsText = stringutils::rangetoStr(*(Result.SourceManager), CallOp->getSourceRange());
            // Extract just the parameter list
            size_t Start = ParamsText.find('(');
            size_t End = ParamsText.find(')');
            if (Start != std::string::npos && End != std::string::npos) {
              ParamsText = ParamsText.substr(Start, End - Start + 1);
            }
          } else {
            ParamsText = "()";
          }
        }
        
        // Check for captures
        std::string CaptureText;
        if (LE->getCaptureDefault() != LCD_None || LE->capture_size() > 0) {
          // Build capture list
          CaptureText = "[";
          if (LE->getCaptureDefault() == LCD_ByCopy)
            CaptureText += "=";
          else if (LE->getCaptureDefault() == LCD_ByRef)
            CaptureText += "&";
          
          for (unsigned i = 0; i < LE->capture_size(); ++i) {
            LambdaCapture Capture = LE->capture_begin()[i];
            if (Capture.capturesVariable()) {
              VarDecl *VD = Capture.getCapturedVar();
              if (VD) {
                if (i > 0 || LE->getCaptureDefault() != LCD_None)
                  CaptureText += ", ";
                CaptureText += VD->getNameAsString();
                if (Capture.getCaptureKind() == LCK_ByRef)
                  CaptureText += "&";
              }
            }
          }
          CaptureText += "]";
        }
        
        // Create functor struct
        std::string FunctorName = "LocalFunctor_" + FD->getNameAsString();
        std::string FunctorStruct = "struct " + FunctorName + " {\n";
        
        // Add captured variables as members if any
        if (!CaptureText.empty() && CaptureText != "[]") {
          // Extract captured variable names
          std::vector<std::string> CapturedVars;
          size_t Start = CaptureText.find('[');
          size_t End = CaptureText.find(']');
          if (Start != std::string::npos && End != std::string::npos) {
            std::string Captures = CaptureText.substr(Start + 1, End - Start - 1);
            if (Captures != "" && Captures != "=" && Captures != "&") {
              size_t Pos = 0;
              while (Pos < Captures.length()) {
                size_t NextComma = Captures.find(',', Pos);
                std::string Var = Captures.substr(Pos, NextComma == std::string::npos ? Captures.length() - Pos : NextComma - Pos);
                // Remove trailing '&' if present
                if (Var.back() == '&')
                  Var.pop_back();
                CapturedVars.push_back(Var);
                if (NextComma == std::string::npos)
                  break;
                Pos = NextComma + 1;
              }
            }
          }
          
          // Add member variables for captures
          for (const std::string &Var : CapturedVars) {
            FunctorStruct += "    " + Var + ";\n";
          }
          
          // Add constructor if there are captures
          FunctorStruct += "    " + FunctorName + "(";
          for (size_t i = 0; i < CapturedVars.size(); ++i) {
            FunctorStruct += CapturedVars[i];
            if (i < CapturedVars.size() - 1)
              FunctorStruct += ", ";
          }
          FunctorStruct += ") : ";
          for (size_t i = 0; i < CapturedVars.size(); ++i) {
            FunctorStruct += CapturedVars[i] + "(" + CapturedVars[i] + ")";
            if (i < CapturedVars.size() - 1)
              FunctorStruct += ", ";
          }
          FunctorStruct += " {}\n";
        }
        
        // Add operator()
        FunctorStruct += "    auto operator()" + ParamsText + " {\n";
        FunctorStruct += "        " + LambdaBody + "\n";
        FunctorStruct += "    }\n";
        FunctorStruct += "};\n";
        
        //Replace the original AST node with the mutated one
        std::string Replacement = FunctorName + "{}";
        Rewrite.ReplaceText(LambdaRange, Replacement);
        
        // Insert functor struct before the field declaration
        SourceLocation InsertLoc = FD->getBeginLoc();
        Rewrite.InsertTextBefore(InsertLoc, FunctorStruct);
      }
    }
}
  
void MutatorFrontendAction_12::MutatorASTConsumer_12::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = fieldDecl(hasInClassInitializer(hasDescendant(lambdaExpr()))).bind("FieldDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}