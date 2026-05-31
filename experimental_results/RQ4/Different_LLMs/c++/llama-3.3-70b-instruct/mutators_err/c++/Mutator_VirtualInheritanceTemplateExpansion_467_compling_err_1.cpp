//header file
#pragma once
#include "Mutator_base.h"

/**
 * VirtualInheritanceTemplateExpansion_467
 */ 
class MutatorFrontendAction_VirtualInheritanceTemplateExpansion_467 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(VirtualInheritanceTemplateExpansion_467)

private:
    class MutatorASTConsumer_VirtualInheritanceTemplateExpansion_467 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_VirtualInheritanceTemplateExpansion_467(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_VirtualInheritanceTemplateExpansion_467.h"

// ========================================================================================================
#define MUT467_OUTPUT 1

void MutatorFrontendAction_VirtualInheritanceTemplateExpansion_467::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("CXXRecordDecl")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      // Add or remove a virtual base class
      if (MT->getNumBases() > 0) {
        // Remove a virtual base class
        auto bases = MT->bases();
        if (bases.size() > 1) {
          auto base = bases[1];
          if (base->isVirtual()) {
            declaration.erase(declaration.find("virtual"), 7);
          }
        }
      } else {
        // Add a virtual base class
        declaration.insert(declaration.find("public"), " virtual ");
      }
      // Introduce or modify the scope resolution operator for member access
      if (MT->getNumMethods() > 0) {
        auto methods = MT->methods();
        auto method = methods[0];
        if (method->getNameAsString() != "") {
          declaration.insert(declaration.find(method->getNameAsString()), "::");
        }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}

void MutatorFrontendAction_VirtualInheritanceTemplateExpansion_467::MutatorASTConsumer_VirtualInheritanceTemplateExpansion_467::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl(hasAnyBase(hasType(cxxRecordDecl()))).bind("CXXRecordDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}