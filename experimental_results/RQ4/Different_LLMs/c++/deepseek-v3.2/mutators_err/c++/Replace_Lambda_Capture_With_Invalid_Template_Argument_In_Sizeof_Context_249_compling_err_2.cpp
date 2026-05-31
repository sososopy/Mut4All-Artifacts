//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Lambda_Capture_With_Invalid_Template_Argument_In_Sizeof_Context_249
 */ 
class MutatorFrontendAction_249 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(249)

private:
    class MutatorASTConsumer_249 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_249(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::VarDecl*> incompleteTypeVars;
        std::vector<const clang::LambdaExpr*> candidateLambdas;
    };
};

//source file
#include "../include/Mutator_249.h"

// ========================================================================================================
#define MUT249_OUTPUT 1

void MutatorFrontendAction_249::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("IncompleteVar")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      //Record the node information to be used in the mutation process
      auto type = VD->getType();
      if (type->isIncompleteType()) {
        incompleteTypeVars.push_back(VD);
      }
    }
    else if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("LambdaExpr")) {
      //Filter nodes in header files
      if (!LE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LE->getBeginLoc()))
        return;
      //Record the node information to be used in the mutation process
      candidateLambdas.push_back(LE);
    }
    else if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("CallExpr")) {
      //Filter nodes in header files
      if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CE->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto callText = stringutils::rangetoStr(*(Result.SourceManager), CE->getSourceRange());
      //Check if this call expression uses a lambda as argument and performs sizeof on template parameter
      bool hasSizeof = false;
      for (auto child : CE->children()) {
        if (auto *UE = dyn_cast<UnaryExprOrTypeTraitExpr>(child)) {
          if (UE->getKind() == UETT_SizeOf) {
            hasSizeof = true;
            break;
          }
        }
      }
      if (!hasSizeof) return;
      
      //Find a lambda among candidates that is used in this call
      for (auto *LE : candidateLambdas) {
        if (LE->getBeginLoc() < CE->getBeginLoc() && LE->getEndLoc() > CE->getBeginLoc()) {
          //Perform mutation on the source code text by applying string replacement
          if (incompleteTypeVars.empty()) return;
          size_t idx = getrandom::getRandomIndex(incompleteTypeVars.size() - 1);
          auto *targetVar = incompleteTypeVars[idx];
          auto lambdaText = stringutils::rangetoStr(*(Result.SourceManager), LE->getSourceRange());
          //Replace lambda capture with incomplete type variable
          size_t captureStart = lambdaText.find('[');
          size_t captureEnd = lambdaText.find(']');
          if (captureStart != std::string::npos && captureEnd != std::string::npos) {
            std::string newCapture = "[" + targetVar->getNameAsString() + "]";
            lambdaText.replace(captureStart, captureEnd - captureStart + 1, newCapture);
            //Replace the original AST node with the mutated one
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(LE->getSourceRange()), lambdaText);
          }
          break;
        }
      }
    }
}
  
void MutatorFrontendAction_249::MutatorASTConsumer_249::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher incompleteVarMatcher = varDecl(hasType(isIncomplete())).bind("IncompleteVar");
    StatementMatcher lambdaMatcher = lambdaExpr().bind("LambdaExpr");
    StatementMatcher callMatcher = callExpr(hasAnyArgument(hasType(isIncomplete()))).bind("CallExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(incompleteVarMatcher, &callback);
    matchFinder.addMatcher(lambdaMatcher, &callback);
    matchFinder.addMatcher(callMatcher, &callback);
    matchFinder.matchAST(Context);
}