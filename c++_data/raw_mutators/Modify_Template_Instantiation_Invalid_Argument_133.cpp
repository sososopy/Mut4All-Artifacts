//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Instantiation_Invalid_Argument_133
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
#include "../include/modify_template_instantiation_invalid_argument_133.h"

// ========================================================================================================
#define MUT133_OUTPUT 1

void MutatorFrontendAction_133::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("TemplateInstance")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;

      //Get the source code text of target node
      auto templateArgs = MT->getTemplateArgs().asArray();
      if (templateArgs.size() != 1)
        return;

      const TemplateArgument &arg = templateArgs[0];
      if (arg.getKind() != TemplateArgument::Type)
        return;

      QualType originalType = arg.getAsType();
      if (originalType->isIntegralType(*Result.Context)) {
        //Perform mutation on the source code text by applying string replacement
        std::string replacementType = "double";
        std::string originalTypeStr = originalType.getAsString();
        SourceRange range = MT->getSourceRange();
        auto sourceText = stringutils::rangetoStr(*(Result.SourceManager), range);
        size_t pos = sourceText.find(originalTypeStr);
        if (pos != std::string::npos) {
          sourceText.replace(pos, originalTypeStr.length(), replacementType);
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(range), sourceText);
        }
      }
    }
}
  
void MutatorFrontendAction_133::MutatorASTConsumer_133::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateSpecializationDecl(hasAnyTemplateArgument(refersToType(isInteger()))).bind("TemplateInstance");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}