```cpp
//header file
#pragma once
#include "Mutator_base.h"

/**
 * TemplateInstantiationOrderMutator_113
 */ 
class MutatorFrontendAction_TemplateInstantiationOrderMutator_113 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(TemplateInstantiationOrderMutator_113)

private:
    class MutatorASTConsumer_TemplateInstantiationOrderMutator_113 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_TemplateInstantiationOrderMutator_113(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_TemplateInstantiationOrderMutator_113.h"

// ========================================================================================================
#define MUTTEMPLATEINSTANTIATIONORDERMUTATOR_113_OUTPUT 1

void MutatorFrontendAction_TemplateInstantiationOrderMutator_113::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("TemplateInstantiation")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      // Randomly shuffle the template arguments
      std::vector<std::string> templateArgs;
      for (const auto &arg : MT->templateArguments()) {
        templateArgs.push_back(arg.getAsType().getAsString());
      }
      std::random_shuffle(templateArgs.begin(), templateArgs.end());
      std::string mutatedDeclaration = declaration;
      size_t pos = mutatedDeclaration.find('<');
      while (pos != std::string::npos) {
        size_t endPos = mutatedDeclaration.find('>', pos);
        if (endPos == std::string::npos) break;
        std::string args = mutatedDeclaration.substr(pos + 1, endPos - pos - 1);
        std::vector<std::string> argsVec;
        size_t argPos = 0;
        while (argPos < args.size()) {
          size_t commaPos = args.find(',', argPos);
          if (commaPos == std::string::npos) {
            argsVec.push_back(args.substr(argPos));
            break;
          }
          argsVec.push_back(args.substr(argPos, commaPos - argPos));
          argPos = commaPos + 1;
        }
        for (size_t i = 0; i < argsVec.size(); ++i) {
          argsVec[i] = templateArgs[i];
        }
        std::string mutatedArgs;
        for (const auto &arg : argsVec) {
          mutatedArgs += arg + ",";
        }
        mutatedArgs.pop_back(); // Remove the trailing comma
        mutatedDeclaration.replace(pos + 1, endPos - pos - 1, mutatedArgs);
        pos = mutatedDeclaration.find('<', endPos);
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedDeclaration);
    }
}
  
void MutatorFrontendAction_TemplateInstantiationOrderMutator_113::MutatorASTConsumer_TemplateInstantiationOrderMutator_113::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateSpecializationDecl().bind("TemplateInstantiation");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}