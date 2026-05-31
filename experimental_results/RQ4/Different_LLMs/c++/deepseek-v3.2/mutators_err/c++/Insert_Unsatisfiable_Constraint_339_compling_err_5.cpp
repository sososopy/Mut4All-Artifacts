//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Unsatisfiable_Constraint_339
 */ 
class MutatorFrontendAction_339 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(339)

private:
    class MutatorASTConsumer_339 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_339(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const CXXRecordDecl *> forwardDecls;
        std::vector<const ConceptDecl *> conceptDecls;
        std::vector<const RequiresExpr *> requiresExprs;
    };
};

//source file
#include "../include/Mutator_Insert_Unsatisfiable_Constraint_339.h"

// ========================================================================================================
#define MUT339_OUTPUT 1

void MutatorFrontendAction_339::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<CXXRecordDecl>("ForwardDecl")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Record forward declarations
      if (!FD->isCompleteDefinition()) {
        forwardDecls.push_back(FD);
      }
    }
    else if (auto *CD = Result.Nodes.getNodeAs<ConceptDecl>("ConceptDecl")) {
      //Filter nodes in header files
      if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CD->getLocation()))
        return;
      //Record concept declarations
      conceptDecls.push_back(CD);
    }
    else if (auto *RE = Result.Nodes.getNodeAs<RequiresExpr>("RequiresExpr")) {
      //Filter nodes in header files
      if (!RE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     RE->getBeginLoc()))
        return;
      //Record requires expressions
      requiresExprs.push_back(RE);
    }
    else if (auto *MT = Result.Nodes.getNodeAs<ConceptDecl>("MutateConcept")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto conceptText = stringutils::rangetoStr(*(Result.SourceManager),
                                                 MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::string forwardTypeName;
      if (forwardDecls.empty()) {
        // Insert a new forward declaration before the concept
        forwardTypeName = "__mutated_type";
        Rewrite.InsertTextBefore(MT->getBeginLoc(), "class __mutated_type;\n");
      } else {
        size_t idx = getrandom::getRandomIndex(forwardDecls.size() - 1);
        forwardTypeName = forwardDecls[idx]->getNameAsString();
      }
      // Add unsatisfiable constraint referencing forward-declared type
      std::string newConstraint = " && requires { typename " + forwardTypeName + "::nonexistent; }";
      // Insert constraint at appropriate location (end of concept body)
      size_t pos = conceptText.find(';');
      if (pos != std::string::npos) {
        conceptText.insert(pos, newConstraint);
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), conceptText);
    }
    else if (auto *MT = Result.Nodes.getNodeAs<RequiresExpr>("MutateRequires")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto requiresText = stringutils::rangetoStr(*(Result.SourceManager),
                                                  MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::string forwardTypeName;
      if (forwardDecls.empty()) {
        // Insert a new forward declaration before the requires expression
        forwardTypeName = "__mutated_type";
        Rewrite.InsertTextBefore(MT->getBeginLoc(), "class __mutated_type;\n");
      } else {
        size_t idx = getrandom::getRandomIndex(forwardDecls.size() - 1);
        forwardTypeName = forwardDecls[idx]->getNameAsString();
      }
      // Add unsatisfiable requirement referencing forward-declared type
      std::string newRequirement = " { " + forwardTypeName + "{} } -> std::convertible_to<int>; ";
      // Insert requirement inside requires clause
      size_t pos = requiresText.find('}');
      if (pos != std::string::npos) {
        requiresText.insert(pos, newRequirement);
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), requiresText);
    }
}
  
void MutatorFrontendAction_339::MutatorASTConsumer_339::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher forwardMatcher = cxxRecordDecl(unless(isDefinition())).bind("ForwardDecl");
    DeclarationMatcher conceptMatcher = conceptDecl(isExpansionInMainFile()).bind("ConceptDecl");
    StatementMatcher requiresMatcher = requiresExpr(isExpansionInMainFile()).bind("RequiresExpr");
    DeclarationMatcher mutateConceptMatcher = conceptDecl(isExpansionInMainFile()).bind("MutateConcept");
    StatementMatcher mutateRequiresMatcher = requiresExpr(isExpansionInMainFile()).bind("MutateRequires");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(forwardMatcher, &callback);
    matchFinder.addMatcher(conceptMatcher, &callback);
    matchFinder.addMatcher(requiresMatcher, &callback);
    matchFinder.addMatcher(mutateConceptMatcher, &callback);
    matchFinder.addMatcher(mutateRequiresMatcher, &callback);
    matchFinder.matchAST(Context);
}