//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Concept_Requires_Call_To_Constrained_Function_68
 */ 
class MutatorFrontendAction_68 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(68)

private:
    class MutatorASTConsumer_68 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_68(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::FunctionDecl*> constrainedFunctions;
        std::vector<const clang::ConceptDecl*> conceptDecls;
    };
};

//source file
#include "../include/Mutator_68.h"

// ========================================================================================================
#define MUT68_OUTPUT 1

void MutatorFrontendAction_68::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("constrainedFunction")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Record constrained function declarations
      if (FD->getTrailingRequiresClause()) {
        constrainedFunctions.push_back(FD);
      }
    }
    else if (auto *CD = Result.Nodes.getNodeAs<clang::ConceptDecl>("conceptDecl")) {
      //Filter nodes in header files
      if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CD->getLocation()))
        return;
      //Record concept declarations
      conceptDecls.push_back(CD);
    }
    else if (auto *RE = Result.Nodes.getNodeAs<clang::Expr>("requiresExpr")) {
      //Filter nodes in header files
      if (!RE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     RE->getExprLoc()))
        return;
      //Get the source code text of target node
      auto requiresText = stringutils::rangetoStr(*(Result.SourceManager), RE->getSourceRange());
      
      //Find the concept this requires-expression belongs to
      const clang::ConceptDecl* parentConcept = nullptr;
      for (auto* CD : conceptDecls) {
        if (CD->getBody()) {
          // Check if RE is within the concept body by comparing source locations
          if (Result.SourceManager->isPointWithin(RE->getBeginLoc(), CD->getBody()->getBeginLoc(), CD->getBody()->getEndLoc())) {
            parentConcept = CD;
            break;
          }
        }
      }
      
      if (!parentConcept) return;
      
      //Look for a call expression within the requires-expression
      const clang::CallExpr* callExpr = nullptr;
      for (auto* child : RE->children()) {
        if (auto* CE = llvm::dyn_cast<clang::CallExpr>(child)) {
          callExpr = CE;
          break; //Take the first call expression found
        }
      }
      
      if (!callExpr) return;
      
      //Find a constrained function to replace the call
      const clang::FunctionDecl* targetFunction = nullptr;
      for (auto* FD : constrainedFunctions) {
        //Prefer functions constrained by the same concept if available
        auto* requiresClause = FD->getTrailingRequiresClause();
        if (requiresClause) {
          //Check if the constraint references our concept
          std::string constraintText = stringutils::rangetoStr(*(Result.SourceManager), 
                                                               requiresClause->getSourceRange());
          if (constraintText.find(parentConcept->getNameAsString()) != std::string::npos) {
            targetFunction = FD;
            break;
          }
        }
      }
      
      //If no constrained function exists, create a placeholder
      if (!targetFunction) {
        //Create a new function declaration before the concept
        std::string newFunc = "template<typename T>\n";
        newFunc += "auto mut_68_func(T t) requires " + parentConcept->getNameAsString() + "<T> {\n";
        newFunc += "    return t;\n}\n";
        
        //Insert the new function before the concept
        Rewrite.InsertTextBefore(parentConcept->getBeginLoc(), newFunc);
        
        //Now use this new function in the requires-expression
        targetFunction = nullptr; //We'll reference it by name
      }
      
      //Perform mutation on the source code text by applying string replacement
      std::string oldCallText = stringutils::rangetoStr(*(Result.SourceManager), callExpr->getSourceRange());
      std::string newCallText;
      
      if (targetFunction) {
        newCallText = targetFunction->getNameAsString() + "(/*mut68*/0)";
      } else {
        newCallText = "mut_68_func(/*mut68*/0)";
      }
      
      //Replace the call expression in the requires-expression
      size_t pos = requiresText.find(oldCallText);
      if (pos != std::string::npos) {
        requiresText.replace(pos, oldCallText.length(), newCallText);
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(clang::CharSourceRange::getTokenRange(RE->getSourceRange()), requiresText);
      }
    }
}
  
void MutatorFrontendAction_68::MutatorASTConsumer_68::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher constrainedFuncMatcher = functionDecl(hasTrailingRequiresClause()).bind("constrainedFunction");
    DeclarationMatcher conceptMatcher = conceptDecl().bind("conceptDecl");
    StatementMatcher requiresMatcher = requiresExpr().bind("requiresExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(constrainedFuncMatcher, &callback);
    matchFinder.addMatcher(conceptMatcher, &callback);
    matchFinder.addMatcher(requiresMatcher, &callback);
    matchFinder.matchAST(Context);
}