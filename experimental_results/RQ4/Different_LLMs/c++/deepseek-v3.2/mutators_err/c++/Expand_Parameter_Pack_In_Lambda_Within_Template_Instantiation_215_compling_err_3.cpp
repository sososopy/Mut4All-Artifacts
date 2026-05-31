//header file
#pragma once
#include "Mutator_base.h"

/**
 * Expand_Parameter_Pack_In_Lambda_Within_Template_Instantiation_215
 */ 
class MutatorFrontendAction_215 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(215)

private:
    class MutatorASTConsumer_215 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_215(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Expand_Parameter_Pack_In_Lambda_Within_Template_Instantiation_215.h"

// ========================================================================================================
#define MUT215_OUTPUT 1

void MutatorFrontendAction_215::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *L = Result.Nodes.getNodeAs<clang::LambdaExpr>("LambdaExpr")) {
      //Filter nodes in header files
      if (!L || !Result.Context->getSourceManager().isWrittenInMainFile(
                     L->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto lambdaRange = L->getSourceRange();
      auto lambdaText = stringutils::rangetoStr(*(Result.SourceManager), lambdaRange);
      //Find enclosing template context
      auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl");
      if (!FD || !FD->getTemplateSpecializationInfo())
        return;
      //Check for parameter pack
      auto *TP = Result.Nodes.getNodeAs<clang::TemplateTypeParmDecl>("TemplateTypeParmDecl");
      if (!TP || !TP->isParameterPack())
        return;
      //Get pack name
      std::string packName = TP->getNameAsString();
      if (packName.empty())
        return;
      //Determine operator for fold expression
      std::string op = "+";
      //Check lambda body for existing operator
      auto *body = L->getBody();
      if (body) {
        //Traverse body to find binary operators
        for (auto *child : body->children()) {
          if (auto *BO = dyn_cast<BinaryOperator>(child)) {
            op = BO->getOpcodeStr().str();
            break;
          }
        }
      }
      //Perform mutation on the source code text by applying string replacement
      //Modify lambda capture to include pack
      std::string mutatedLambda = lambdaText;
      //Find capture list start
      size_t capturePos = mutatedLambda.find("[");
      if (capturePos != std::string::npos) {
        size_t captureEnd = mutatedLambda.find("]", capturePos);
        if (captureEnd != std::string::npos) {
          std::string newCapture = mutatedLambda.substr(capturePos, captureEnd - capturePos + 1);
          //Add pack expansion to capture
          if (newCapture == "[]") {
            newCapture = "[&" + packName + "...]";
          } else {
            newCapture.insert(newCapture.length() - 1, "&" + packName + "...");
          }
          mutatedLambda.replace(capturePos, captureEnd - capturePos + 1, newCapture);
        }
      }
      //Replace a statement in lambda body with fold expression
      size_t bodyStart = mutatedLambda.find("{");
      if (bodyStart != std::string::npos) {
        size_t bodyEnd = mutatedLambda.find("}", bodyStart);
        if (bodyEnd != std::string::npos) {
          std::string bodyContent = mutatedLambda.substr(bodyStart + 1, bodyEnd - bodyStart - 1);
          //Find a simple statement to replace
          size_t stmtPos = bodyContent.find("return");
          if (stmtPos != std::string::npos) {
            size_t stmtEnd = bodyContent.find(";", stmtPos);
            if (stmtEnd != std::string::npos) {
              std::string foldExpr = "return (" + packName + " " + op + " ...);";
              bodyContent.replace(stmtPos, stmtEnd - stmtPos + 1, foldExpr);
              mutatedLambda.replace(bodyStart + 1, bodyEnd - bodyStart - 1, bodyContent);
            }
          } else {
            //Insert fold expression as new statement
            std::string foldExpr = "(" + packName + " " + op + " ...);";
            bodyContent.insert(0, foldExpr);
            mutatedLambda.replace(bodyStart + 1, bodyEnd - bodyStart - 1, bodyContent);
          }
        }
      }
      mutatedLambda = "/*mut215*/" + mutatedLambda;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(lambdaRange), mutatedLambda);
    }
}
  
void MutatorFrontendAction_215::MutatorASTConsumer_215::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(
      hasDescendant(
        templateTypeParmDecl(
          isParameterPack()
        ).bind("TemplateTypeParmDecl")
      ),
      hasDescendant(
        lambdaExpr().bind("LambdaExpr")
      )
    ).bind("FunctionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}