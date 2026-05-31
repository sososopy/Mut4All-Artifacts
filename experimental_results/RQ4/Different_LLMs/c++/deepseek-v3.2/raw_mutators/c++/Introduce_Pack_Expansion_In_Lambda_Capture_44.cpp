//header file
#pragma once
#include "Mutator_base.h"

/**
 * Introduce_Pack_Expansion_In_Lambda_Capture_44
 */ 
class MutatorFrontendAction_44 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(44)

private:
    class MutatorASTConsumer_44 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_44(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result);
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Mutator_Introduce_Pack_Expansion_In_Lambda_Capture_44.h"

// ========================================================================================================
#define MUT44_OUTPUT 1

void MutatorFrontendAction_44::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::LambdaExpr>("LambdaExpr")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      
      // Check if lambda is inside a template context
      auto *DC = MT->getCallOperator()->getDeclContext();
      bool inTemplate = false;
      while (DC) {
        if (isa<FunctionDecl>(DC)) {
          auto *FD = cast<FunctionDecl>(DC);
          if (FD->getTemplatedKind() != FunctionDecl::TK_NonTemplate) {
            inTemplate = true;
            break;
          }
        } else if (isa<ClassTemplateSpecializationDecl>(DC) || 
                   isa<ClassTemplatePartialSpecializationDecl>(DC)) {
          inTemplate = true;
          break;
        }
        DC = DC->getParent();
      }
      if (!inTemplate) return;
      
      // Check if lambda has a parameter pack
      auto *CallOp = MT->getCallOperator();
      if (!CallOp->isParameterPack()) {
        bool hasPackParam = false;
        for (auto *P : CallOp->parameters()) {
          if (P->isParameterPack()) {
            hasPackParam = true;
            break;
          }
        }
        if (!hasPackParam) return;
      }
      
      // Check if lambda already captures a pack expansion
      auto *Lambda = MT->getLambdaClass();
      for (auto *F : Lambda->fields()) {
        if (F->getIdentifier() && 
            F->getName().endswith("...")) {
          return; // Already has pack capture
        }
      }
      
      //Get the source code text of target node
      auto LambdaRange = MT->getSourceRange();
      auto LambdaText = stringutils::rangetoStr(*(Result.SourceManager), LambdaRange);
      
      // Find parameter pack name
      std::string PackName;
      for (auto *P : CallOp->parameters()) {
        if (P->isParameterPack()) {
          PackName = P->getNameAsString();
          break;
        }
      }
      if (PackName.empty()) return;
      
      // Find the capture list
      size_t BracketPos = LambdaText.find('[');
      if (BracketPos == std::string::npos) return;
      size_t BracketEnd = LambdaText.find(']', BracketPos);
      if (BracketEnd == std::string::npos) return;
      
      // Modify capture list
      std::string NewCapture;
      std::string CaptureContent = LambdaText.substr(BracketPos + 1, BracketEnd - BracketPos - 1);
      if (CaptureContent.empty()) {
        NewCapture = "[" + PackName + "...]";
      } else {
        NewCapture = "[" + CaptureContent + ", " + PackName + "...]";
      }
      
      // Find lambda body
      size_t BodyStart = LambdaText.find('{', BracketEnd);
      if (BodyStart == std::string::npos) return;
      size_t BodyEnd = LambdaText.rfind('}');
      if (BodyEnd == std::string::npos) return;
      
      // Check if body has return statement
      std::string Body = LambdaText.substr(BodyStart + 1, BodyEnd - BodyStart - 1);
      size_t ReturnPos = Body.find("return");
      if (ReturnPos != std::string::npos) {
        // Replace return expression with fold expression
        size_t ReturnEnd = Body.find(';', ReturnPos);
        if (ReturnEnd != std::string::npos) {
          std::string NewBody = Body.substr(0, ReturnPos) + 
                               "return (" + PackName + " + ...);" + 
                               Body.substr(ReturnEnd + 1);
          LambdaText.replace(BodyStart + 1, BodyEnd - BodyStart - 1, NewBody);
        }
      } else {
        // Insert fold expression at beginning of body
        std::string NewBody = "(" + PackName + " + ...); " + Body;
        LambdaText.replace(BodyStart + 1, BodyEnd - BodyStart - 1, NewBody);
      }
      
      // Replace capture list
      LambdaText.replace(BracketPos, BracketEnd - BracketPos + 1, NewCapture);
      
      // Add comment
      LambdaText = "/*mut44*/" + LambdaText;
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(LambdaRange), LambdaText);
    }
}
  
void MutatorFrontendAction_44::MutatorASTConsumer_44::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = lambdaExpr().bind("LambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}