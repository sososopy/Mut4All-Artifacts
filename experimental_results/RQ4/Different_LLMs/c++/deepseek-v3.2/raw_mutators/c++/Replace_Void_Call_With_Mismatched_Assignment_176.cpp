//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Void_Call_With_Mismatched_Assignment_176
 */ 
class MutatorFrontendAction_176 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(176)

private:
    class MutatorASTConsumer_176 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_176(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<std::string> availableTypes;
    };
};

//source file
#include "../include/Mutator_.h"

// ========================================================================================================
#define MUT176_OUTPUT 1

void MutatorFrontendAction_176::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("callExpr")) {
      //Filter nodes in header files
      if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CE->getLocation()))
        return;

      // Check if the function returns void
      QualType returnType = CE->getCallReturnType(*Result.Context);
      if (!returnType->isVoidType())
        return;

      // Get the enclosing statement
      const Stmt *parent = Result.Nodes.getNodeAs<clang::Stmt>("parentStmt");
      if (!parent)
        return;

      // Get source range of the statement
      SourceRange stmtRange = parent->getSourceRange();
      if (stmtRange.isInvalid())
        return;

      // Determine a suitable non-void type for assignment
      std::string typeName;
      if (availableTypes.empty()) {
        // Fallback to basic types if no user-defined types found
        typeName = "int";
      } else {
        size_t idx = getrandom::getRandomIndex(availableTypes.size() - 1);
        typeName = availableTypes[idx];
      }

      // Generate variable name
      std::string varName = "mut176_var_" + std::to_string(getrandom::getRandomIndex(1000));

      // Get the call expression text
      std::string callText = stringutils::rangetoStr(*(Result.SourceManager), CE->getSourceRange());

      // Construct the new assignment statement
      std::string newStmt = typeName + " " + varName + " = " + callText + ";";

      // Add mutation marker
      newStmt = "/*mut176*/" + newStmt;

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(stmtRange), newStmt);
    }
    else if (auto *RD = Result.Nodes.getNodeAs<clang::RecordDecl>("recordDecl")) {
      // Collect user-defined types for potential use
      if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     RD->getLocation()))
        return;

      if (RD->isCompleteDefinition()) {
        std::string typeName = RD->getNameAsString();
        if (!typeName.empty()) {
          availableTypes.push_back(typeName);
        }
      }
    }
}
  
void MutatorFrontendAction_176::MutatorASTConsumer_176::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher callMatcher = callExpr(unless(hasAncestor(compoundStmt()))).bind("callExpr");
    StatementMatcher parentMatcher = stmt(hasDescendant(callExpr().bind("callExpr"))).bind("parentStmt");
    DeclarationMatcher recordMatcher = recordDecl().bind("recordDecl");
    
    Callback callback(TheRewriter);
    matchFinder.addMatcher(callMatcher, &callback);
    matchFinder.addMatcher(parentMatcher, &callback);
    matchFinder.addMatcher(recordMatcher, &callback);
    matchFinder.matchAST(Context);
}