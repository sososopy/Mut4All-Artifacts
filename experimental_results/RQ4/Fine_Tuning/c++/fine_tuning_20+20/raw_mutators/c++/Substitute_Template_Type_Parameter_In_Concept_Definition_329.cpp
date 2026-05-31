//header file
#pragma once
#include "Mutator_base.h"

/**
 * Substitute_Template_Type_Parameter_In_Concept_Definition_329
 */ 
class MutatorFrontendAction_329 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(329)

private:
    class MutatorASTConsumer_329 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_329(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Substitute_Template_Type_Parameter_In_Concept_Definition_329.h"

// ========================================================================================================
#define MUT329_OUTPUT 1

void MutatorFrontendAction_329::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ConceptDecl>("Concepts")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto definition = stringutils::rangetoStr(*(Result.SourceManager),
                                                MT->getSourceRange());
      llvm::outs() << definition << '\n';
      //Perform mutation on the source code text by applying string replacement
      if (definition.find("decltype") != string::npos) {
        auto pos = definition.find("decltype");
        definition.insert(pos, "/*mut329*/");
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()),
                            definition);
        return;
      }
      auto params = MT->getTemplateParameters();
      std::vector<string> param_names;
      for (auto param : *params) {
        if (auto type_param = dyn_cast<TemplateTypeParmDecl>(param)) {
          param_names.push_back(type_param->getNameAsString());
        }
      }
      llvm::outs() << "param_names size: " << param_names.size() << '\n';
      for (auto param : param_names) {
        auto pos = definition.find(param);
        if (pos != string::npos) {
          definition.replace(pos, param.length(), "decltype(" + param + ")");
        }
      }
      definition = "/*mut329*/" + definition;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()),
                          definition);
    }
}
  
void MutatorFrontendAction_329::MutatorASTConsumer_329::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = conceptDecl().bind("Concepts");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}