//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Member_Init_With_Variable_Constructor_360
 */ 
class MutatorFrontendAction_360 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(360)

private:
    class MutatorASTConsumer_360 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_360(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::FieldDecl*> memberVariables;
    };
};

//source file
#include "../include/Mutator_Replace_Member_Init_With_Variable_Constructor_360.h"

// ========================================================================================================
#define MUT360_OUTPUT 1

void MutatorFrontendAction_360::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FieldDecl>("MemberVar")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Record the node information to be used in the mutation process
      memberVariables.push_back(FD);
    }
    else if (auto *CS = Result.Nodes.getNodeAs<clang::CompoundStmt>("CompoundStmt")) {
      //Filter nodes in header files
      if (!CS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CS->getBeginLoc()))
        return;
      //Get the source code text of target node
      if (memberVariables.empty()) return;
      
      // Find a suitable statement inside the compound statement to replace
      const clang::Stmt* targetStmt = nullptr;
      for (const clang::Stmt* child : CS->children()) {
        if (child && (isa<clang::Expr>(child) || isa<clang::DeclStmt>(child))) {
          targetStmt = child;
          break;
        }
      }
      if (!targetStmt) return;
      
      // Select a random member variable from recorded list
      size_t idx = getrandom::getRandomIndex(memberVariables.size() - 1);
      const clang::FieldDecl* selectedMember = memberVariables[idx];
      std::string memberName = selectedMember->getNameAsString();
      
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedText = memberName + "({});";
      mutatedText = "/*mut360*/" + mutatedText;
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(targetStmt->getSourceRange()), mutatedText);
    }
}
  
void MutatorFrontendAction_360::MutatorASTConsumer_360::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher memberMatcher = fieldDecl().bind("MemberVar");
    StatementMatcher compoundMatcher = compoundStmt().bind("CompoundStmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(memberMatcher, &callback);
    matchFinder.addMatcher(compoundMatcher, &callback);
    matchFinder.matchAST(Context);
}