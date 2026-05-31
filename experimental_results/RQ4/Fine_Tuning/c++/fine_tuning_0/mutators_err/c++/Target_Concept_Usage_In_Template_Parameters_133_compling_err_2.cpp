//header file
#pragma once
#include "Mutator_base.h"

/**
 * Target_Concept_Usage_In_Template_Parameters_133
 */ 
class MutatorFrontendAction_133 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(133)

private:
    class MutatorASTConsumer_133 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_133(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Target_Concept_Usage_In_Template_Parameters_133.h"

// ========================================================================================================
#define MUT133_OUTPUT 1

void MutatorFrontendAction_133::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateDecl>("Template")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             MT->getSourceRange());
      llvm::outs() << content << '\n';
      auto params = MT->getTemplateParameters();
      int param_size = params->size();
      if (param_size == 0)
        return;
      int index = getrandom::getRandomIndex(param_size - 1);
      auto param = params->getParam(index);
      if (!isa<TemplateTypeParmDecl>(param))
        return;
      auto type_param = dyn_cast<TemplateTypeParmDecl>(param);
      if (type_param->hasTypeConstraint())
        return;
      auto concepts = Result.Context->getTranslationUnitDecl()->decls();
      std::vector<string> concept_names;
      for (auto concept : concepts) {
        if (concept->getKind() == Decl::Kind::Concept) {
          auto concept_name = dyn_cast<ConceptDecl>(concept)->getNameAsString();
          concept_names.push_back(concept_name);
        }
      }
      if (concept_names.empty())
        return;
      int concept_index = getrandom::getRandomIndex(concept_names.size() - 1);
      auto concept_name = concept_names[concept_index];
      llvm::outs() << concept_name << '\n';
      auto pos = content.find("typename " + type_param->getNameAsString());
      if (pos != string::npos)
        content.replace(pos, 8, concept_name);
      content = "/*mut133*/" + content;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()),
                          content);
    }
}
  
void MutatorFrontendAction_133::MutatorASTConsumer_133::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = clang::ast_matchers::templateDecl().bind("Template");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}