```cpp
//header file
#pragma once
#include "Mutator_base.h"

/**
 * TemplateInstantiationWithInheritanceChange_152
 */ 
class MutatorFrontendAction_152 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(152)

private:
    class MutatorASTConsumer_152 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_152(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl *> baseClasses;
    };
};

//source file
#include "../include/Mutator_TemplateInstantiationWithInheritanceChange_152.h"

// ========================================================================================================
#define MUT152_OUTPUT 1

void MutatorFrontendAction_152::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      //Filter nodes in header files
      if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CL->getLocation()))
        return;
      //Record the candidate existing class names for insertion
      baseClasses.push_back(CL);
    }

    //Check whether the matched AST node is the target node
    else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Derived")) {
      //Filter nodes in header files
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      //Get the source code text of target node
      auto bases = DL->bases();
      for (auto base : bases) {
        if (base->isTemplateInstantiation()) {
          //Perform mutation on the source code text by applying string replacement
          string className = base->getType().getAsString();
          for (auto baseClass : baseClasses) {
            if (baseClass->getNameAsString() != className) {
              string newBaseClass = baseClass->getNameAsString();
              string newBaseClassType = newBaseClass + "<" + base->getType().getTemplateName().getAsString() + ">";
              //Replace the original AST node with the mutated one
              Rewrite.ReplaceText(CharSourceRange::getTokenRange(base->getSourceRange()), newBaseClassType);
            }
          }
        }
      }
    }
}

void MutatorFrontendAction_152::MutatorASTConsumer_152::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define ASTMatchers to identify the target AST node for mutation
    //Find the base classes to be inserted
    auto base_matcher = cxxRecordDecl().bind("Classes");
    //Find the derived classes to be mutated
    auto derived_matcher = cxxRecordDecl(hasAnyBase(hasType(cxxRecordDecl()))).bind("Derived");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(base_matcher, &callback);
    matchFinder.addMatcher(derived_matcher, &callback);
    matchFinder.matchAST(Context);
}