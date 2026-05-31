//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Var_With_Member_Access_291
 */ 
class MutatorFrontendAction_291 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(291)

private:
    class MutatorASTConsumer_291 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_291(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::RecordDecl*> structDecls;
        std::vector<const clang::VarDecl*> structVars;
        std::vector<const clang::DeclRefExpr*> candidateIdExprs;
    };
};

//source file
#include "../include/Mutator_291.h"

// ========================================================================================================
#define MUT291_OUTPUT 1

void MutatorFrontendAction_291::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *RD = Result.Nodes.getNodeAs<clang::RecordDecl>("StructDecl")) {
      //Filter nodes in header files
      if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     RD->getLocation()))
        return;
      //Record the node information to be used in the mutation process
      structDecls.push_back(RD);
    }
    else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("StructVar")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      //Record the node information to be used in the mutation process
      if (VD->getType()->isRecordType()) {
        structVars.push_back(VD);
      }
    }
    else if (auto *DRE = Result.Nodes.getNodeAs<clang::DeclRefExpr>("IdExpr")) {
      //Filter nodes in header files
      if (!DRE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DRE->getLocation()))
        return;
      //Record the node information to be used in the mutation process
      candidateIdExprs.push_back(DRE);
    }
    else if (auto *CS = Result.Nodes.getNodeAs<clang::CompoundStmt>("CompoundStmt")) {
      //Filter nodes in header files
      if (!CS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CS->getLocation()))
        return;
      
      //Perform mutation on the source code text by applying string replacement
      //First, collect all struct definitions and struct variables in this compound statement
      std::vector<const clang::RecordDecl*> localStructDecls;
      std::vector<const clang::VarDecl*> localStructVars;
      for (auto RD : structDecls) {
        if (Result.Context->getSourceManager().isPointWithin(RD->getBeginLoc(), CS->getBeginLoc(), CS->getEndLoc())) {
          localStructDecls.push_back(RD);
        }
      }
      for (auto VD : structVars) {
        if (Result.Context->getSourceManager().isPointWithin(VD->getBeginLoc(), CS->getBeginLoc(), CS->getEndLoc())) {
          localStructVars.push_back(VD);
        }
      }
      
      //Find candidate identifier expressions within this compound statement
      std::vector<const clang::DeclRefExpr*> localIdExprs;
      for (auto DRE : candidateIdExprs) {
        if (Result.Context->getSourceManager().isPointWithin(DRE->getBeginLoc(), CS->getBeginLoc(), CS->getEndLoc())) {
          localIdExprs.push_back(DRE);
        }
      }
      
      if (localIdExprs.empty()) return;
      
      //Select one identifier expression to mutate
      size_t idx = getrandom::getRandomIndex(localIdExprs.size() - 1);
      const clang::DeclRefExpr* targetDRE = localIdExprs[idx];
      
      //Get the source code text of target node
      std::string idName = targetDRE->getNameInfo().getName().getAsString();
      
      //Determine if we need to add a struct variable
      clang::VarDecl* structVar = nullptr;
      if (localStructVars.empty()) {
        //Need to create a struct variable
        if (localStructDecls.empty()) {
          //No struct definition in scope - create one
          std::string structName = "S_" + idName;
          std::string structDef = "\nstruct " + structName + " { int " + idName + "; };";
          Rewrite.InsertText(CS->getBeginLoc().getLocWithOffset(1), structDef);
          //Create struct variable declaration
          std::string varDecl = "\nstruct " + structName + " s_" + idName + " = {0};";
          Rewrite.InsertText(CS->getBeginLoc().getLocWithOffset(1), varDecl);
          structVar = nullptr; // We'll use the name we inserted
        } else {
          //Use existing struct definition
          const clang::RecordDecl* chosenStruct = localStructDecls[getrandom::getRandomIndex(localStructDecls.size() - 1)];
          std::string structName = chosenStruct->getNameAsString();
          //Check if struct has member with same name
          bool hasMember = false;
          for (auto field : chosenStruct->fields()) {
            if (field->getNameAsString() == idName) {
              hasMember = true;
              break;
            }
          }
          if (!hasMember) {
            //Add member to struct definition
            std::string memberAdd = " int " + idName + ";";
            Rewrite.InsertText(chosenStruct->getEndLoc().getLocWithOffset(-1), memberAdd);
          }
          //Create struct variable declaration
          std::string varDecl = "\nstruct " + structName + " s_" + idName + " = {0};";
          Rewrite.InsertText(CS->getBeginLoc().getLocWithOffset(1), varDecl);
          structVar = nullptr;
        }
      } else {
        //Use existing struct variable
        structVar = localStructVars[getrandom::getRandomIndex(localStructVars.size() - 1)];
      }
      
      //Replace the original AST node with the mutated one
      std::string replacement;
      if (structVar) {
        replacement = structVar->getNameAsString() + "." + idName;
      } else {
        replacement = "s_" + idName + "." + idName;
      }
      Rewrite.ReplaceText(targetDRE->getSourceRange(), replacement);
    }
}
  
void MutatorFrontendAction_291::MutatorASTConsumer_291::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher structDeclMatcher = recordDecl().bind("StructDecl");
    DeclarationMatcher structVarMatcher = varDecl().bind("StructVar");
    StatementMatcher idExprMatcher = declRefExpr().bind("IdExpr");
    StatementMatcher compoundStmtMatcher = compoundStmt().bind("CompoundStmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(structDeclMatcher, &callback);
    matchFinder.addMatcher(structVarMatcher, &callback);
    matchFinder.addMatcher(idExprMatcher, &callback);
    matchFinder.addMatcher(compoundStmtMatcher, &callback);
    matchFinder.matchAST(Context);
}