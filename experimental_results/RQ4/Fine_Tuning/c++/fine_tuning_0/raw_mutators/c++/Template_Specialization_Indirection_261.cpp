//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Specialization_Indirection_261
 */ 
class MutatorFrontendAction_261 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(261)

private:
    class MutatorASTConsumer_261 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_261(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Template_Specialization_Indirection_261.h"

// ========================================================================================================
#define MUT261_OUTPUT 1

void MutatorFrontendAction_261::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TS = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("TemplateSpecialization")) {
      if (!TS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TS->getLocation()))
        return;
      if (!TS->isCompleteDefinition())
        return;
      auto inline_methods = TS->methods();
      if (inline_methods.empty())
        return;
      for (auto method : inline_methods) {
        if (method->isImplicit())
          continue;
        if (!method->isInlined())
          continue;
        auto method_name = method->getNameAsString();
        auto method_source = stringutils::rangetoStr(
            *(Result.SourceManager), method->getSourceRange());
        auto method_return_type = method->getReturnType();
        if (method_return_type->isBuiltinType())
          continue;
        auto method_return_type_name = method_return_type.getAsString();
        llvm::outs() << method_return_type_name << '\n';
        auto method_return_type_loc = method->getReturnTypeSourceRange();
        auto method_return_type_pointee = method_return_type_name + " *";
        // auto method_return_type_pointee = method_return_type_name + " &";
        llvm::outs() << method_return_type_pointee << '\n';
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(
                                method_return_type_loc),
                            method_return_type_pointee);
        auto method_body = method->getBody();
        auto method_body_source = stringutils::rangetoStr(
            *(Result.SourceManager), method_body->getSourceRange());
        llvm::outs() << method_body_source << '\n';
        if (method_body_source.find("return") == string::npos)
          continue;
        auto return_pos = method_body_source.find("return");
        method_body_source.insert(return_pos + 6, "*");
        llvm::outs() << method_body_source << '\n';
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(
                                method_body->getSourceRange()),
                            method_body_source);
      }
    }
}
  
void MutatorFrontendAction_261::MutatorASTConsumer_261::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateSpecializationDecl().bind("TemplateSpecialization");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}