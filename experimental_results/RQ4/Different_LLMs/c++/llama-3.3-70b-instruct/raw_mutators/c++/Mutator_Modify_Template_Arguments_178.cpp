//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Arguments_178
 */ 
class MutatorFrontendAction_178 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(178)

private:
    class MutatorASTConsumer_178 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_178(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Modify_Template_Arguments_178.h"

// ========================================================================================================
#define MUT178_OUTPUT 1

void MutatorFrontendAction_178::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("TemplateInstantiations")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      // Replace one of the template arguments with an inappropriate or unexpected type
      std::string mutatedDeclaration = declaration;
      size_t templateArgStart = declaration.find('<');
      size_t templateArgEnd = declaration.find('>');
      if (templateArgStart != std::string::npos && templateArgEnd != std::string::npos) {
        std::string templateArgs = declaration.substr(templateArgStart + 1, templateArgEnd - templateArgStart - 1);
        size_t commaPos = templateArgs.find(',');
        if (commaPos != std::string::npos) {
          // Replace the first template argument with 'void (*)()'
          mutatedDeclaration = declaration.substr(0, templateArgStart + 2) + "void (*)()" + declaration.substr(templateArgStart + 2 + commaPos);
        } else {
          // Replace the only template argument with 'std::vector<int>'
          mutatedDeclaration = declaration.substr(0, templateArgStart + 2) + "std::vector<int>" + declaration.substr(templateArgEnd);
        }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedDeclaration);
    }
}
  
void MutatorFrontendAction_178::MutatorASTConsumer_178::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateSpecializationDecl().bind("TemplateInstantiations");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}