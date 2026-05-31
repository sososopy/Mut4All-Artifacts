//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Struct_Member_Access_With_Invalid_Member_Access_In_Aggregate_Initializer_373
 */ 
class MutatorFrontendAction_373 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(373)

private:
    class MutatorASTConsumer_373 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_373(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl*> structDecls;
        std::vector<std::string> memberNames;
    };
};

//source file
#include "../include/Mutator_Replace_Struct_Member_Access_With_Invalid_Member_Access_In_Aggregate_Initializer_373.h"

// ========================================================================================================
#define MUT373_OUTPUT 1

void MutatorFrontendAction_373::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::InitListExpr>("InitListExpr")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      
      // Check if this is an aggregate initializer
      if (!MT->isAggregateInitialization())
        return;
      
      // Iterate through initializers
      for (unsigned i = 0; i < MT->getNumInits(); ++i) {
        Expr* initExpr = MT->getInit(i);
        if (!initExpr) continue;
        
        // Check if this is a member access expression (either -> or .)
        if (auto* memberExpr = dyn_cast<MemberExpr>(initExpr)) {
          // Get the accessed member name
          ValueDecl* memberDecl = memberExpr->getMemberDecl();
          if (!memberDecl) continue;
          
          std::string originalMemberName = memberDecl->getNameAsString();
          
          // Get the object being accessed
          Expr* baseExpr = memberExpr->getBase();
          if (!baseExpr) continue;
          
          // Determine the type of the object
          QualType baseType = baseExpr->getType();
          if (baseType->isPointerType())
            baseType = baseType->getPointeeType();
          
          // Get the struct/class type
          const CXXRecordDecl* recordDecl = baseType->getAsCXXRecordDecl();
          if (!recordDecl) continue;
          
          // Collect all member names from this record
          std::vector<std::string> validMembers;
          for (FieldDecl* field : recordDecl->fields()) {
            validMembers.push_back(field->getNameAsString());
          }
          
          // Find an invalid member name
          std::string invalidMemberName;
          bool foundInvalid = false;
          
          // First try to use a member name from another struct in the program
          for (const std::string& candidate : memberNames) {
            // Check if candidate is not a member of the current record
            if (std::find(validMembers.begin(), validMembers.end(), candidate) == validMembers.end()) {
              invalidMemberName = candidate;
              foundInvalid = true;
              break;
            }
          }
          
          // If no existing invalid name found, generate a new one
          if (!foundInvalid) {
            // Generate a simple identifier that's not in validMembers
            std::string newName = "invalid_member";
            int counter = 1;
            while (std::find(validMembers.begin(), validMembers.end(), newName) != validMembers.end()) {
              newName = "invalid_member_" + std::to_string(counter);
              counter++;
            }
            invalidMemberName = newName;
          }
          
          // Get the source code text of target node
          std::string originalText = stringutils::rangetoStr(*(Result.SourceManager), memberExpr->getSourceRange());
          
          // Find the position of the member name in the expression
          // We need to replace just the member name part
          std::string baseText = stringutils::rangetoStr(*(Result.SourceManager), baseExpr->getSourceRange());
          std::string accessOperator = memberExpr->isArrow() ? "->" : ".";
          
          // Construct new member access expression
          std::string newText = baseText + accessOperator + invalidMemberName;
          
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(memberExpr->getSourceRange()), newText);
        }
      }
    }
    // Also collect struct declarations to gather member names
    else if (auto *RD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("StructDecl")) {
      if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     RD->getLocation()))
        return;
      
      // Only collect struct/class definitions
      if (!RD->isCompleteDefinition())
        return;
      
      // Store the declaration
      structDecls.push_back(RD);
      
      // Collect member names from this struct
      for (FieldDecl* field : RD->fields()) {
        memberNames.push_back(field->getNameAsString());
      }
    }
}
  
void MutatorFrontendAction_373::MutatorASTConsumer_373::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    // Match aggregate initializers
    StatementMatcher initMatcher = initListExpr(isAggregateInitialization()).bind("InitListExpr");
    // Match struct/class declarations to collect member names
    DeclarationMatcher structMatcher = cxxRecordDecl().bind("StructDecl");
    
    Callback callback(TheRewriter);
    matchFinder.addMatcher(initMatcher, &callback);
    matchFinder.addMatcher(structMatcher, &callback);
    matchFinder.matchAST(Context);
}