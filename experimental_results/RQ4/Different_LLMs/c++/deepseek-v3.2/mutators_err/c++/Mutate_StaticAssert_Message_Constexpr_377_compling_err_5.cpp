//header file
#pragma once
#include "Mutator_base.h"

/**
 * Mutate_StaticAssert_Message_Constexpr_377
 */ 
class MutatorFrontendAction_377 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(377)

private:
    class MutatorASTConsumer_377 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_377(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const CXXRecordDecl*> targetTypes;
    };
};

//source file
#include "../include/Mutate_StaticAssert_Message_Constexpr_377.h"

// ========================================================================================================
#define MUT377_OUTPUT 1

void MutatorFrontendAction_377::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *SA = Result.Nodes.getNodeAs<clang::StaticAssertDecl>("StaticAssert")) {
      //Filter nodes in header files
      if (!SA || !Result.Context->getSourceManager().isWrittenInMainFile(
                     SA->getLocation()))
        return;
      
      // Get the second argument (message)
      const Expr* messageExpr = SA->getMessage();
      if (!messageExpr) return;
      
      // Check if message is a constructor call of a user-defined type
      const CXXConstructExpr* constructExpr = dyn_cast<CXXConstructExpr>(messageExpr);
      if (!constructExpr) return;
      
      const CXXRecordDecl* recordDecl = constructExpr->getConstructor()->getParent();
      if (!recordDecl) return;
      
      // Record this type for later processing
      targetTypes.push_back(recordDecl);
    }
    else if (auto *RD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("RecordDecl")) {
      //Filter nodes in header files
      if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     RD->getLocation()))
        return;
      
      // Check if this is one of our target types
      bool isTarget = false;
      for (auto target : targetTypes) {
        if (target == RD) {
          isTarget = true;
          break;
        }
      }
      if (!isTarget) return;
      
      if (!RD->isCompleteDefinition()) return;
      
      //Get the source code text of target node
      auto recordText = stringutils::rangetoStr(*(Result.SourceManager), RD->getSourceRange());
      
      // Look for constexpr member functions returning integer or pointer types
      bool hasConst2exprMember = false;
      std::vector<CXXMethodDecl*> candidateMethods;
      
      for (auto decl : RD->decls()) {
        if (CXXMethodDecl* method = dyn_cast<CXXMethodDecl>(decl)) {
          if (method->isConstexpr()) {
            QualType returnType = method->getReturnType();
            if (returnType->isIntegerType() || returnType->isPointerType()) {
              candidateMethods.push_back(method);
              hasConstexprMember = true;
            }
          }
        }
      }
      
      //Perform mutation on the source code text by applying string replacement
      if (hasConstexprMember && candidateMethods.size() > 0) {
        // Choose a random method to mutate
        size_t idx = getrandom::getRandomIndex(candidateMethods.size() - 1);
        CXXMethodDecl* method = candidateMethods[idx];
        
        // Get method source range
        SourceRange methodRange = method->getSourceRange();
        auto methodText = stringutils::rangetoStr(*(Result.SourceManager), methodRange);
        
        // Find the return statement
        Stmt* body = method->getBody();
        if (!body) return;
        
        ReturnStmt* returnStmt = nullptr;
        for (auto child : body->children()) {
          if (ReturnStmt* rs = dyn_cast<ReturnStmt>(child)) {
            returnStmt = rs;
            break;
          }
        }
        
        if (!returnStmt) return;
        
        // Get return expression source
        Expr* retExpr = returnStmt->getRetValue();
        if (!retExpr) return;
        
        SourceRange retRange = retExpr->getSourceRange();
        auto retText = stringutils::rangetoStr(*(Result.SourceManager), retRange);
        
        QualType returnType = method->get2ReturnType();
        std::string newRetValue;
        
        if (returnType->isIntegerType()) {
          // Check if unsigned - if unsigned, we can't use negative value
          if (returnType->isUnsignedIntegerType()) {
            // For unsigned, use 0 instead of negative
            newRetValue = "0";
          } else {
            newRetValue = "-1";
          }
        } else if (returnType->isPointerType()) {
          newRetValue = "nullptr";
        } else {
          return;
        }
        
        // Replace return expression
        std::string mutatedMethod = methodText;
        size_t retPos = mutatedMethod.find(retText);
        if (retPos != std::string::npos) {
          mutatedMethod.replace(retPos, retText.length(), newRetValue);
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(methodRange), mutatedMethod);
        }
      } else {
        // No suitable constexpr member function exists - introduce one
        // Determine appropriate return type
        std::string returnTypeStr = "int";
        std::string returnValue = "-1";
        
        // Insert new member function at end of class (before closing brace)
        std::string newMember = "\n\tconstexpr " + returnTypeStr + " size() const { return " + returnValue + "; }";
        size_t bracePos = recordText.rfind('}');
        if (bracePos != std::string::npos) {
          recordText.insert(bracePos, newMember);
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(RD->getSourceRange()), recordText);
        }
      }
    }
}
  
void MutatorFrontendAction_377::MutatorASTConsumer_377::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher staticAssertMatcher = staticAssertDecl().bind("StaticAssert");
    DeclarationMatcher recordMatcher = cxxRecordDecl().bind("RecordDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(staticAssertMatcher, &callback);
    matchFinder.addMatcher(recordMatcher, &callback);
    matchFinder.matchAST(Context);
}