//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_AliasTemplate_With_DeducedType_139
 */ 
class MutatorFrontendAction_139 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(139)

private:
    class MutatorASTConsumer_139 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_139(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/replace_aliastemplate_with_deducedtype_139.h"

// ========================================================================================================
#define MUT139_OUTPUT 1

void MutatorFrontendAction_139::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *AL = Result.Nodes.getNodeAs<clang::TypeAliasDecl>("Alias")) {
      //Filter nodes in header files
      if (!AL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     AL->getLocation()))
        return;
      if (AL->getTypeSourceInfo() == nullptr)
        return;
      if (AL->getTypeSourceInfo()->getTypeLoc().getTypeLocClass() !=
          TypeLoc::TypeLocClass::TemplateSpecialization)
        return;
      auto ALTP = AL->getTypeSourceInfo()
                      ->getTypeLoc()
                      .getAs<TemplateSpecializationTypeLoc>();
      if (ALTP.getTypePtr() == nullptr)
        return;
      if (ALTP.getTypePtr()->isTypeAlias() == false)
        return;
      if (ALTP.getTypePtr()->getTemplateName().getKind() !=
          TemplateName::NameKind::UsingTemplate)
        return;
      auto ALTPDL = ALTP.getTypePtr()->getAs<TemplateSpecializationType>();
      if (ALTPDL == nullptr)
        return;
      auto ALTPDLTP = ALTPDL->getTemplateName();
      if (ALTPDLTP.getKind() != TemplateName::NameKind::UsingTemplate)
        return;
      auto ALTPDLTPDL = ALTPDLTP.getAsUsingShadowDecl();
      if (ALTPDLTPDL == nullptr)
        return;
      auto ALTPDLTPDLTT = ALTPDLTPDL->getUnderlyingDecl();
      if (ALTPDLTPDLTT == nullptr)
        return;
      if (ALTPDLTPDLTT->getKind() != Decl::Kind::TypeAliasTemplate)
        return;
      auto ALTPDLTPDLTTDL =
          dyn_cast<TypeAliasTemplateDecl>(ALTPDLTPDLTT)->getTemplatedDecl();
      if (ALTPDLTPDLTTDL == nullptr)
        return;
      auto ALTPDLTPDLTTDLTT = ALTPDLTPDLTTDL->getTypeSourceInfo();
      if (ALTPDLTPDLTTDLTT == nullptr)
        return;
      if (ALTPDLTPDLTTDLTT->getTypeLoc().getTypeLocClass() !=
          TypeLoc::TypeLocClass::TemplateSpecialization)
        return;
      auto ALTPDLTPDLTTDLTTDL =
          ALTPDLTPDLTTDLTT->getTypeLoc().getAs<TemplateSpecializationTypeLoc>();
      if (ALTPDLTPDLTTDLTTDL.getTypePtr() == nullptr)
        return;
      if (ALTPDLTPDLTTDLTTDL.getTypePtr()->getTemplateName().getKind() !=
          TemplateName::NameKind::Template)
        return;
      auto ALTPDLTPDLTTDLTTDLTP =
          ALTPDLTPDLTTDLTTDL.getTypePtr()->getTemplateName();
      if (ALTPDLTPDLTTDLTTDLTP.getKind() != TemplateName::NameKind::Template)
        return;
      auto ALTPDLTPDLTTDLTTDLTPDL =
          ALTPDLTPDLTTDLTTDLTP.getAsTemplateDecl();
      if (ALTPDLTPDLTTDLTTDLTPDL == nullptr)
        return;
      if (ALTPDLTPDLTTDLTTDLTPDL->getKind() != Decl::Kind::ClassTemplate)
        return;
      auto ALTPDLTPDLTTDLTTDLTPDLTP =
          dyn_cast<ClassTemplateDecl>(ALTPDLTPDLTTDLTTDLTPDL);
      if (ALTPDLTPDLTTDLTTDLTPDLTP == nullptr)
        return;
      auto ALTPDLTPDLTTDLTTDLTPDLTPDL =
          ALTPDLTPDLTTDLTTDLTPDLTP->getTemplatedDecl();
      if (ALTPDLTPDLTTDLTTDLTPDLTPDL == nullptr)
        return;
      if (ALTPDLTPDLTTDLTTDLTPDLTPDL->getKind() != Decl::Kind::CXXRecord)
        return;
      auto ALTPDLTPDLTTDLTTDLTPDLTPDLTP =
          dyn_cast<CXXRecordDecl>(ALTPDLTPDLTTDLTTDLTPDLTPDL);
      if (ALTPDLTPDLTTDLTTDLTPDLTPDLTP == nullptr)
        return;
      if (ALTPDLTPDLTTDLTTDLTPDLTPDLTP->isCompleteDefinition() == false)
        return;
      if (ALTPDLTPDLTTDLTTDLTPDLTPDLTP->isStruct() == false)
        return;
      auto ALTPDLTPDLTTDLTTDLTPDLTPDLTPDL =
          ALTPDLTPDLTTDLTTDLTPDLTPDLTP->getDefinition();
      if (ALTPDLTPDLTTDLTTDLTPDLTPDLTPDL == nullptr)
        return;
      if (ALTPDLTPDLTTDLTTDLTPDLTPDLTPDL->isCompleteDefinition() == false)
        return;
      if (ALTPDLTPDLTTDLTTDLTPDLTPDLTPDL->isStruct() == false)
        return;
      auto ALTPDLTPDLTTDLTTDLTPDLTPDLTPDLTT =
          ALTPDLTPDLTTDLTTDLTPDLTPDLTPDL->getTypeForDecl();
      if (ALTPDLTPDLTTDLTTDLTPDLTPDLTPDLTT == nullptr)
        return;
      auto ALTPDLTPDLTTDLTTDLTPDLTPDLTPDLTTDL =
          ALTPDLTPDLTTDLTTDLTPDLTPDLTPDLTT->getCanonicalTypeInternal();
      if (ALTPDLTPDLTTDLTTDLTPDLTPDLTPDLTTDL.isNull())
        return;
      auto ALTPDLTPDLTTDLTTDLTPDLTPDLTPDLTTDLTT =
          ALTPDLTPDLTTDLTTDLTPDLTPDLTPDLTTDL.getAsString();
      if (ALTPDLTPDLTTDLTTDLTPDLTPDLTPDLTTDLTT.empty())
        return;
      auto ALTPDLTPDLTTDLTTDLTPDLTPDLTPDLTTDLTTDL =
          ALTPDLTPDLTTDLTTDLTPDLTPDLTPDLTTDLTT + "{}";
      //Perform mutation on the source code text by applying string replacement
      auto ALTPDLTPDLTTDLTTDLTPDLTPDLTPDLTTDLTTDLTT =
          "/*mut139*/" + ALTPDLTPDLTTDLTTDLTPDLTPDLTPDLTTDLTTDL;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(AL->getSourceRange(), ALTPDLTPDLTTDLTTDLTPDLTPDLTPDLTTDLTTDLTT);
    }
}
  
void MutatorFrontendAction_139::MutatorASTConsumer_139::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = typeAliasDecl().bind("Alias");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}