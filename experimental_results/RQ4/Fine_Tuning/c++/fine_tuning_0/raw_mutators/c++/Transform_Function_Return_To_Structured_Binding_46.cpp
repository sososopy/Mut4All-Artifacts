//header file
#pragma once
#include "Mutator_base.h"

/**
 * Transform_Function_Return_To_Structured_Binding_46
 */ 
class MutatorFrontendAction_46 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(46)

private:
    class MutatorASTConsumer_46 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_46(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Transform_Function_Return_To_Structured_Binding_46.h"

// ========================================================================================================
#define MUT46_OUTPUT 1

void MutatorFrontendAction_46::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (!FD->hasBody())
        return;
      if (FD->isMain())
        return;
      if (FD->getReturnType()->isVoidType())
        return;
      if (FD->getReturnType()->isBuiltinType())
        return;
      if (FD->getReturnType()->isPointerType())
        return;
      if (FD->getReturnType()->isReferenceType())
        return;
      if (FD->getReturnType()->isEnumeralType())
        return;
      if (FD->getReturnType()->isUnionType())
        return;
      if (FD->getReturnType()->isArrayType())
        return;
      if (FD->getReturnType()->isFunctionType())
        return;
      if (FD->getReturnType()->isIncompleteType())
        return;
      if (FD->getReturnType()->isDependentType())
        return;
      if (FD->getReturnType()->isUndeducedType())
        return;
      if (FD->getReturnType()->isTypedefNameType())
        return;
      if (FD->getReturnType()->isElaboratedType())
        return;
      if (FD->getReturnType()->isInjectedClassNameType())
        return;
      if (FD->getReturnType()->isTemplateTypeParmType())
        return;
      if (FD->getReturnType()->isSubstTemplateTypeParmType())
        return;
      if (FD->getReturnType()->isSubstTemplateTypeParmPackType())
        return;
      if (FD->getReturnType()->isAutoType())
        return;
      if (FD->getReturnType()->isDeducedTemplateSpecializationType())
        return;
      if (FD->getReturnType()->isPackExpansionType())
        return;
      if (FD->getReturnType()->isObjCObjectType())
        return;
      if (FD->getReturnType()->isObjCInterfaceType())
        return;
      if (FD->getReturnType()->isObjCObjectPointerType())
        return;
      if (FD->getReturnType()->isAtomicType())
        return;
      if (FD->getReturnType()->isPipeType())
        return;
      if (FD->getReturnType()->isBitIntType())
        return;
      if (FD->getReturnType()->isDependentBitIntType())
        return;
      if (FD->getReturnType()->isBTFTagAttributedType())
        return;
      if (FD->getReturnType()->isRecordType() == false)
        return;
      auto returntype = FD->getReturnType()->getAsRecordDecl();
      if (returntype->isCompleteDefinition() == false)
        return;
      auto fields = returntype->fields();
      for (auto field : fields) {
        if (field->getType()->isReferenceType())
          return;
      }
      auto body = FD->getBody();
      auto body_str = stringutils::rangetoStr(*(Result.SourceManager),
                                              body->getSourceRange());
      auto name = FD->getNameAsString();
      auto ret_type = FD->getReturnType().getAsString();
      string auto_decl = "const auto& [";
      for (auto field : fields) {
        auto_decl += field->getNameAsString() + ",";
      }
      auto_decl.pop_back();
      auto_decl += "] = ";
      auto_decl += name + "();\n";
      auto_decl = "/*mut46*/" + auto_decl;
      llvm::outs() << auto_decl << '\n';
      auto_decl += body_str;
      auto_decl += "\n/*mut46*/[&](){return ";
      for (auto field : fields) {
        auto_decl += field->getNameAsString() + "==";
      }
      auto_decl.pop_back();
      auto_decl += ";}();";
      llvm::outs() << auto_decl << '\n';
      Rewrite.ReplaceText(body->getBeginLoc(), 0, auto_decl);
    }
}
  
void MutatorFrontendAction_46::MutatorASTConsumer_46::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl().bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}