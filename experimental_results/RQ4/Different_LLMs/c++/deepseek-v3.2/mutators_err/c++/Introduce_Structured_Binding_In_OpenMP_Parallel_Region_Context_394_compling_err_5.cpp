//header file
#pragma once
#include "Mutator_base.h"

/**
 * Introduce_Structured_Binding_In_OpenMP_Parallel_Region_Context_394
 */ 
class MutatorFrontendAction_394 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(394)

private:
    class MutatorASTConsumer_394 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_394(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::VarDecl*> structuredBindings;
        std::vector<const clang::DeclRefExpr*> bindingReferences;
    };
};

//source file
#include "../include/Introduce_Structured_Binding_In_OpenMP_Parallel_Region_Context_394.h"

// ========================================================================================================
#define MUT394_OUTPUT 1

void MutatorFrontendAction_394::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("StructuredBinding")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      //Record structured binding declarations
      if (VD->getType()->isStructureType()) {
        structuredBindings.push_back(VD);
      }
    }
    else if (auto *DRE = Result.Nodes.getNodeAs<clang::DeclRefExpr>("BindingRef")) {
      //Filter nodes in header files
      if (!DRE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DRE->getLocation()))
        return;
      //Record references to structured binding variables
      const clang::VarDecl* VD = llvm::dyn_cast<clang::VarDecl>(DRE->getDecl());
      if (VD && VD->getType()->isStructureType()) {
        bindingReferences.push_back(DRE);
      }
    }
    else if (auto *FS = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
      //Filter nodes in header files
      if (!FS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FS->getLocation()))
        return;
      //Check if function has a body
      if (!FS->hasBody()) return;
      
      //Find a statement using a structured binding variable within this function
      clang::Stmt* targetStmt = nullptr;
      clang::SourceLocation insertLoc;
      
      for (auto* DRE : bindingReferences) {
        //Check if reference is within this function's body
        if (Result.Context->getSourceManager().isPointWithin(DRE->getBeginLoc(), 
            FS->getBody()->getBeginLoc(), FS->getBody()->getEndLoc())) {
          //Find the statement containing this reference
          const clang::Stmt* parent = DRE;
          while (parent && parent != FS->getBody()) {
            clang::Stmt* nextParent = nullptr;
            for (auto* child : parent->children()) {
              if (child && Result.Context->getSourceManager().isPointWithin(DRE->getBeginLoc(), 
                  child->getBeginLoc(), child->getEndLoc())) {
                nextParent = const_cast<clang::Stmt*>(child);
                break;
              }
            }
            if (!nextParent) break;
            parent = nextParent;
          }
          if (parent && parent != FS->getBody()) {
            targetStmt = const_cast<clang::Stmt*>(parent);
            insertLoc = targetSt1t->getBeginLoc();
            break;
          }
        }
      }
      
      if (!targetStmt) return;
      
      //Check if there's already an OpenMP pragma before this statement
      clang::SourceManager& SM = Result.Context->getSourceManager();
      clang::Token token;
      bool hasOpenMP = false;
      
      //Look for pragma before the statement
      clang::SourceLocation loc = insertLoc;
      while (loc.isValid()) {
        loc = SM.getLocForStartOfFile(SM.getFileID(loc));
        if (!loc.isValid()) break;
        
        bool invalid = false;
        token = clang::Lexer::getRawToken(loc, SM, Result.Context->getLangOpts(), &invalid);
        if (invalid) break;
        
        if (token.getKind() == clang::tok::annot_pragma_openmp) {
          hasOpenMP = true;
          break;
        }
      }
      
      //If no OpenMP pragma exists, insert one
      if (!hasOpenMP) {
        //Get the source code text of target node
        std::string stmtText = stringutils::rangetoStr(SM, targetStmt->getSourceRange());
        
        //Perform mutation on the source code text by applying string replacement
        std::string mutatedText = "#pragma omp parallel\n{\n" + stmtText + "\n}";
        
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(targetStmt->getSourceRange()), mutatedText);
      }
    }
}
  
void MutatorFrontendAction_394::MutatorASTConsumer_394::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher bindingMatcher = varDecl(hasType(recordType())).bind("StructuredBinding");
    StatementMatcher refMatcher = declRefExpr(to(varDecl(hasType(recordType())))).bind("BindingRef");
    DeclarationMatcher functionMatcher = functionDecl().bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(bindingMatcher, &callback);
    matchFinder.addMatcher(refMatcher, &callback);
    matchFinder.addMatcher(functionMatcher, &callback);
    matchFinder.matchAST(Context);
}