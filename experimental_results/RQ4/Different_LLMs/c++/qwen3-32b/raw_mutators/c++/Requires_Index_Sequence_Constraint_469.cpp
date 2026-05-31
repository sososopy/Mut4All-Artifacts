//header file
#pragma once
#include "Mutator_base.h"

/**
 * Requires_Index_Sequence_Constraint_469
 */ 
class MutatorFrontendAction_469 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(469)
private:
    class MutatorASTConsumer_469 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_469(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Requires_Index_Sequence_Constraint_469.h"

// ========================================================================================================
#define MUT469_OUTPUT 1

void MutatorFrontendAction_469::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Templates")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      SourceManager &SM = Result.Context->getSourceManager();
      Rewriter &Rewriter = this->Rewrite;
      TemplateParameterList *Params = MT->getTemplateParameters();
      std::string packName = "Ts"; // default fallback
      for (unsigned i = 0; i < Params->size(); ++i) {
        NamedDecl *Param = Params->getParam(i);
        if (auto *TTP = dyn_cast<TemplateTypeParmDecl>(Param)) {
          if (TTP->isParameterPack()) {
            packName = TTP->getNameAsString();
            break;
          }
        }
      }
      //Perform mutation on the source code text by applying string replacement
      SourceRange TemplateRange = MT->getSourceRange();
      std::string OriginalTemplate = stringutils::rangetoStr(SM, TemplateRange);
      std::string NewTemplate = "template <unsigned I, " + OriginalTemplate.substr(8); // Remove "template <"
      //Replace the original AST node with the mutated one
      Rewriter.ReplaceText(TemplateRange, NewTemplate);
      //Insert requires clause after template parameters
      SourceLocation TemplateEnd = TemplateRange.getEnd();
      std::string RequiresClause = " requires (requires { I < sizeof...(" + packName + "); })";
      Rewriter.InsertText(TemplateEnd, RequiresClause, true, true);
    }
}
  
void MutatorFrontendAction_469::MutatorASTConsumer_469::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateDecl(hasAnyParameterPack(anyOf(templateTypeParameter(), nonTypeTemplateParameter(), templateTemplateParameter()))).bind("Templates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}