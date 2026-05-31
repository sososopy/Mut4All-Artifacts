//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Argument_With_Incomplete_Type_Reference_240
 */ 
class MutatorFrontendAction_240 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(240)

private:
    class MutatorASTConsumer_240 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_240(Rewriter &R) : TheRewriter(R) {}
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
        std::set<std::string> incompleteTypes;
        std::string forwardDeclaration;
    };
};

//source file
#include "../include/Mutator_Replace_Template_Argument_With_Incomplete_Type_Reference_240.h"

// ========================================================================================================
#define MUT240_OUTPUT 1

void MutatorFrontendAction_240::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TS = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("TemplateSpecialization")) {
      //Filter nodes in header files
      if (!TS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TS->getLocation()))
        return;
      if (!TS->isExplicitSpecialization())
        return;
      if (!TS->getSpecializationKind() == clang::TemplateSpecializationKind::TSK_Undeclared)
        return;
      //Get the source code text of target node
      auto specializationRange = TS->getSourceRange();
      if (specializationRange.isInvalid())
        return;
      auto specializationText = stringutils::rangetoStr(*(Result.SourceManager), specializationRange);
      //(optional)Record the node information to be used in the mutation process
      auto templateArgs = TS->getTemplateArgs().asArray();
      if (templateArgs.empty())
        return;
      //Perform mutation on the source code text by applying string replacement
      std::string incompleteTypeName = "IncompleteType240";
      if (incompleteTypes.find(incompleteTypeName) == incompleteTypes.end()) {
        forwardDeclaration = "class " + incompleteTypeName + ";";
        incompleteTypes.insert(incompleteTypeName);
      }
      size_t argIndex = getrandom::getRandomIndex(templateArgs.size() - 1);
      auto arg = templateArgs[argIndex];
      if (arg.getKind() != clang::TemplateArgument::Type)
        return;
      auto argType = arg.getAsType();
      std::string argText;
      if (argType->isBuiltinType()) {
        argText = argType.getAsString();
      } else if (auto *recordType = argType->getAsRecordDecl()) {
        argText = recordType->getNameAsString();
      } else {
        return;
      }
      std::string mutatedText = specializationText;
      size_t pos = mutatedText.find(argText);
      if (pos != std::string::npos) {
        mutatedText.replace(pos, argText.length(), incompleteTypeName);
      }
      //Replace the original AST node with the mutated one
      Rewrite.InsertTextBefore(specializationRange.getBegin(), forwardDeclaration + "\n");
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(specializationRange), mutatedText);
    }
}
  
void MutatorFrontendAction_240::MutatorASTConsumer_240::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateSpecializationDecl().bind("TemplateSpecialization");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}