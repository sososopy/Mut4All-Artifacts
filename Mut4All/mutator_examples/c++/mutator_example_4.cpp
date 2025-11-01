//header file
#pragma once
#include "Mutator_base.h"

/**
 * Construct_Multiple-inheritance_Derived_Class_54
 */ 
class MutatorFrontendAction_54 : public MutatorFrontendAction {
    public:
      MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(54)
    
    private:
      class MutatorASTConsumer_54 : public MutatorASTConsumer {
        public:
          MutatorASTConsumer_54(Rewriter &R) : TheRewriter(R) {}
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
          set<string> visitedRecord;
          string lastRecordName = "";
      };
    };

//source file
#include "../include/construct_multiple-inheritance_derived_class_54"

// ========================================================================================================
#define MUT54_OUTPUT 1

void MutatorFrontendAction_54::Callback::run(const MatchFinder::MatchResult &Result) {
    SourceManager &SM = Rewrite.getSourceMgr();
    //Check whether the matched AST node is the target node
    if (const CXXRecordDecl *recordDecl = Result.Nodes.getNodeAs<CXXRecordDecl>("recordDecl1")) {
      if (!recordDecl)
        return;
      //Filter nodes in header files
      if (!Result.Context->getSourceManager().isWrittenInMainFile(recordDecl->getBeginLoc())) 
        return;
      if ((!recordDecl->isStruct() && !recordDecl->isClass()) || recordDecl->isLambda())
        return;
      if (!recordDecl->isCompleteDefinition())
        return;
      //Record the node information of the first parent
      auto recordName = recordDecl->getNameAsString();
      if (recordName == "")
        return;

      if (visitedRecord.find(recordName) == visitedRecord.end()) {
        visitedRecord.insert(recordName);
        lastRecordName = recordName;
        llvm::errs() <<"last record: "<< lastRecordName <<    "\n";
      }
    }
    else if (const CXXRecordDecl *recordDecl = Result.Nodes.getNodeAs<CXXRecordDecl>("recordDecl2")) {
      if (!recordDecl)
        return;
      if (!Result.Context->getSourceManager().isWrittenInMainFile(recordDecl->getBeginLoc())) 
        return;
      if ((!recordDecl->isStruct() && !recordDecl->isClass()) || recordDecl->isLambda())
        return;
      if (!recordDecl->isCompleteDefinition())
        return;

      if (lastRecordName == "")
        return;
      //Record the node information of the second parent
      auto recordName = recordDecl->getNameAsString();
      if (recordName != lastRecordName)
        return;
      //Get the source code text of target node
      string insParam = "";
      for (string rn : visitedRecord) {
        insParam += (insParam == "") ? ": " : ", ";
        string as = getrandom::getRandomAccessSepcifier();
        insParam += as + " " + rn;
      }
      //Perform mutation on the source code text by applying string replacement
      string insText = "class Mut_54" + insParam + " {}\n";
      SourceLocation insLoc = recordDecl->getEndLoc();
      //Replace the original AST node with the mutated one
      Rewrite.InsertTextAfterToken(insLoc, ";\n/*mut54*/"+insText);
    }
  }

void MutatorFrontendAction_54::MutatorASTConsumer_54::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define ASTMatchers to identify the target AST node for mutation
    //Find the first parent class
    DeclarationMatcher matcher1 = cxxRecordDecl().bind("recordDecl1");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher1, &callback);
    matchFinder.matchAST(Context);
    //Find the second parent class
    MatchFinder finder2;
    DeclarationMatcher matcher2 = cxxRecordDecl().bind("recordDecl2");
    finder2.addMatcher(matcher2, &callback);
    finder2.matchAST(Context);
}
