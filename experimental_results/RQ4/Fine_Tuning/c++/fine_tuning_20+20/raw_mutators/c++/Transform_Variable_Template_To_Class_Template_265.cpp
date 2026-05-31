//header file
#pragma once
#include "Mutator_base.h"

/**
 * Transform_Variable_Template_To_Class_Template_265
 */ 
class MutatorFrontendAction_265 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(265)

private:
    class MutatorASTConsumer_265 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_265(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::ClassTemplateDecl *> cur_classes;
    };
};

//source file
#include "../include/transform_variable_template_to_class_template_265.h"

// ========================================================================================================
#define MUT265_OUTPUT 1

void MutatorFrontendAction_265::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ST = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Structs")) {
      if (!ST || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ST->getLocation()))
        return;
      if (ST->isThisDeclarationADefinition() == false)
        return;
      auto DL = ST->getTemplatedDecl();
      if (DL->isStruct() == false)
        return;
      cur_classes.push_back(ST);
    } else if (auto *VT =
                   Result.Nodes.getNodeAs<clang::VarTemplateDecl>("Variables")) {
      if (!VT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VT->getLocation()))
        return;
      if (VT->isThisDeclarationADefinition() == false)
        return;
      auto DL = VT->getTemplatedDecl();
      if (DL->isStaticDataMember() == false)
        return;
      size_t index = 0;
      for (size_t i = 0; i < cur_classes.size(); ++i) {
        if (VT->isOutOfLine() == false &&
            DL->getBeginLoc() > cur_classes[i]->getBeginLoc() &&
            DL->getEndLoc() < cur_classes[i]->getEndLoc()) {
          index = i;
          break;
        }
      }
      auto DL_ = VT->getTemplatedDecl();
      auto DL_str = stringutils::rangetoStr(*(Result.SourceManager),
                                            DL_->getSourceRange());
      DL_str = "struct " + DL_str;
      DL_str = "/*mut265*/" + DL_str;
      Rewrite.ReplaceText(DL_->getSourceRange(), DL_str);
      if (VT->isOutOfLine() == false) {
        auto specs = VT->specializations();
        for (auto spec : specs) {
          auto spec_str = stringutils::rangetoStr(*(Result.SourceManager),
                                                  spec->getSourceRange());
          spec_str = "struct " + spec_str;
          spec_str = "/*mut265*/" + spec_str;
          Rewrite.ReplaceText(spec->getSourceRange(), spec_str);
        }
      } else {
        auto specs = VT->specializations();
        for (auto spec : specs) {
          auto spec_str = stringutils::rangetoStr(*(Result.SourceManager),
                                                  spec->getSourceRange());
          spec_str = "struct " + spec_str;
          spec_str = "/*mut265*/" + spec_str;
          Rewrite.ReplaceText(spec->getSourceRange(), spec_str);
        }
      }
    }
}

void MutatorFrontendAction_265::MutatorASTConsumer_265::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = varTemplateDecl().bind("Variables");
    auto struct_matcher = classTemplateDecl().bind("Structs");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(struct_matcher, &callback);
    matchFinder.matchAST(Context);
}