//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Template_Parameter_Concept_With_Type_63
 */ 
class MutatorFrontendAction_63 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(63)

private:
    class MutatorASTConsumer_63 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_63(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/replace_template_template_parameter_concept_with_type_63.h"

// ========================================================================================================
#define MUT63_OUTPUT 1

void MutatorFrontendAction_63::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
      //Filter nodes in header files
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      //Get the source code text of target node
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      llvm::outs() << content << '\n';
      if (content.find("template") != string::npos) {
        auto temp_content = content.substr(content.find("template"));
        if (temp_content.find("concept") != string::npos) {
          auto concept_content = temp_content.substr(temp_content.find("concept"));
          if (concept_content.find('>') != string::npos) {
            auto concept_name =
                concept_content.substr(0, concept_content.find('>') + 1);
            llvm::outs() << concept_name << '\n';
            if (content.find(concept_name) != string::npos) {
              content.replace(content.find(concept_name), concept_name.size(),
                              "typename");
              llvm::outs() << content << '\n';
              //Perform mutation on the source code text by applying string replacement
              Rewrite.ReplaceText(
                  CharSourceRange::getTokenRange(DL->getSourceRange()), content);
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_63::MutatorASTConsumer_63::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionTemplateDecl().bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}