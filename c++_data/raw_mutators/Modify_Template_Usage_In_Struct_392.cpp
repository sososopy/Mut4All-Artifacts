//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Usage_In_Struct_392
 */ 
class MutatorFrontendAction_392 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(392)

private:
    class MutatorASTConsumer_392 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_392(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/modify_template_usage_in_struct_392.h"

// ========================================================================================================
#define MUT392_OUTPUT 1

void MutatorFrontendAction_392::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *SD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("StructDecl")) {
      if (!SD || !Result.Context->getSourceManager().isWrittenInMainFile(SD->getLocation()))
        return;

      if (SD->isCompleteDefinition()) {
        std::string structContent = stringutils::rangetoStr(*(Result.SourceManager), SD->getSourceRange());
        auto templateParams = SD->getTemplateInstantiationArgs();
        if (templateParams.size() > 0) {
          // Assuming the first template parameter is an integer
          auto param = templateParams[0].getAsIntegral().getLimitedValue();
          std::string newMember = "ASPtrTy<" + std::to_string(param + 1) + "> OffsetBase; // New member using a modified template argument\n";
          structContent.insert(structContent.rfind('}'), "/*mut392*/" + newMember);
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(SD->getSourceRange()), structContent);
        }
      }
    }
}
  
void MutatorFrontendAction_392::MutatorASTConsumer_392::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(isStruct(), hasTemplateArgument(0, expr().bind("TemplateArg"))).bind("StructDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}