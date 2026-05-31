//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Lambda_Capture_In_Template_141
 */ 
class MutatorFrontendAction_141 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(141)

private:
    class MutatorASTConsumer_141 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_141(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Modify_Lambda_Capture_In_Template_141.h"

// ========================================================================================================
#define MUT141_OUTPUT 1

void MutatorFrontendAction_141::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambda")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      SourceManager &SM = Rewrite.getSourceMgr();
      SourceLocation lambdaLoc = MT->getBeginLoc();
      const FunctionDecl* enclosingFunc = nullptr;
      auto parent = Result.Context->getParents(*MT);
      while (!parent.empty()) {
        if (auto* FD = parent[0].get<FunctionDecl>()) {
          enclosingFunc = FD;
          break;
        }
        parent = Result.Context->getParents(parent[0]);
      }
      if (!enclosingFunc || !enclosingFunc->isTemplated()) return;
      //Find variables in scope before lambda
      vector<const VarDecl*> availableVars;
      auto funcBody = dyn_cast<CompoundStmt>(enclosingFunc->getBody());
      if (!funcBody) return;
      for (auto stmt : funcBody->body()) {
        SourceLocation stmtLoc = stmt->getBeginLoc();
        if (SM.isBeforeInTranslationUnit(stmtLoc, lambdaLoc)) {
          if (auto* declStmt = dyn_cast<DeclStmt>(stmt)) {
            for (auto decl : declStmt->decls()) {
              if (auto* varDecl = dyn_cast<VarDecl>(decl)) {
                availableVars.push_back(varDecl);
              }
            }
          }
        } else {
          break;
        }
      }
      //Perform mutation on the source code text by applying string replacement
      string newCapture;
      string varName;
      string varType;
      bool needInsertVar = false;
      if (!availableVars.empty()) {
        size_t idx = getrandom::getRandomIndex(availableVars.size() - 1);
        const VarDecl* chosenVar = availableVars[idx];
        varName = chosenVar->getNameAsString();
        varType = chosenVar->getType().getAsString();
        newCapture = "[&" + varName + "]";
      } else {
        varName = "mut141_var";
        varType = "int";
        needInsertVar = true;
        newCapture = "[&" + varName + "]";
      }
      //Modify lambda capture and add dummy use
      string lambdaText = stringutils::rangetoStr(SM, MT->getSourceRange());
      size_t captureStart = lambdaText.find('[');
      size_t captureEnd = lambdaText.find(']');
      if (captureStart != string::npos && captureEnd != string::npos) {
        string originalCapture = lambdaText.substr(captureStart, captureEnd - captureStart + 1);
        lambdaText.replace(captureStart, captureEnd - captureStart + 1, newCapture);
        //Add dummy use inside lambda body
        size_t bodyStart = lambdaText.find('{');
        if (bodyStart != string::npos) {
          string dummyUse = "auto mut141_dummy = " + varName + ";";
          lambdaText.insert(bodyStart + 1, dummyUse);
        }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), lambdaText);
      if (needInsertVar) {
        string varDecl = varType + " " + varName + " = 0;";
        Rewrite.InsertTextBefore(lambdaLoc, varDecl);
      }
    }
}
  
void MutatorFrontendAction_141::MutatorASTConsumer_141::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = lambdaExpr(hasAncestor(functionDecl(isTemplateInstantiation()))).bind("lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}