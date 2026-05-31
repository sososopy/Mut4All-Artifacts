//header file
#pragma once
#include "Mutator_base.h"

/**
 * Remove_Using_Declaration_In_Lambda_105
 */ 
class MutatorFrontendAction_105 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(105)

private:
    class MutatorASTConsumer_105 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_105(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Remove_Using_Declaration_In_Lambda_105.h"

// ========================================================================================================
#define MUT105_OUTPUT 1

void MutatorFrontendAction_105::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *UD = Result.Nodes.getNodeAs<clang::UsingDecl>("UsingDecl")) {
      //Filter nodes in header files
      if (!UD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     UD->getLocation()))
        return;
      //Check if the using-declaration is inside a lambda or function body within a concept
      auto *Parent = Result.Context->getParents(*UD)[0].get<clang::Decl>();
      if (!Parent) return;
      
      bool insideConceptLambda = false;
      auto *CurrentDecl = Parent;
      while (CurrentDecl) {
        if (auto *Lambda = dyn_cast<clang::LambdaExpr>(CurrentDecl)) {
          //Check if this lambda is part of a concept definition
          auto *LambdaParent = Result.Context->getParents(*Lambda)[0].get<clang::Decl>();
          if (LambdaParent && isa<clang::ConceptDecl>(LambdaParent)) {
            insideConceptLambda = true;
            break;
          }
        } else if (auto *FD = dyn_cast<clang::FunctionDecl>(CurrentDecl)) {
          //Check if this function is part of a concept definition
          auto *FuncParent = Result.Context->getParents(*FD)[0].get<clang::Decl>();
          if (FuncParent && isa<clang::ConceptDecl>(FuncParent)) {
            insideConceptLambda = true;
            break;
          }
        }
        auto NextParent = Result.Context->getParents(*CurrentDecl);
        if (NextParent.empty()) break;
        CurrentDecl = NextParent[0].get<clang::Decl>();
      }
      
      if (!insideConceptLambda) return;
      
      //Get the source code text of target node
      SourceRange Range = UD->getSourceRange();
      if (!Range.isValid()) return;
      
      //Perform mutation on the source code text by applying string replacement
      //Remove the using-declaration entirely (option 1 from specification)
      Rewrite.ReplaceText(Range, "");
    }
}
  
void MutatorFrontendAction_105::MutatorASTConsumer_105::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = usingDecl(hasAnyUsingShadowDecl(hasTargetDecl(hasName("std::begin")))).bind("UsingDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}