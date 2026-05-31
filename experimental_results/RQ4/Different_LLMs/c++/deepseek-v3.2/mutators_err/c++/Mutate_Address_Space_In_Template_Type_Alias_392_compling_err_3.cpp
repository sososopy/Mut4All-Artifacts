//header file
#pragma once
#include "Mutator_base.h"

/**
 * Mutate_Address_Space_In_Template_Type_Alias_392
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
        //Necessary node information record used in the mutation process
        std::vector<std::string> templateParamNames;
    };
};

//source file
#include "../include/Mutate_Address_Space_In_Template_Type_Alias_392.h"

// ========================================================================================================
#define MUT392_OUTPUT 1

void MutatorFrontendAction_392::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TA = Result.Nodes.getNodeAs<clang::TypeAliasTemplateDecl>("TypeAliasTemplateDecl")) {
      //Filter nodes in header files
      if (!TA || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TA->getLocation()))
        return;
      
      // Check if the type alias contains an address space attribute
      auto *TypeAliasDecl = TA->getTemplatedDecl();
      if (!TypeAliasDecl) return;
      
      // Get the underlying type
      QualType UnderlyingType = TypeAliasDecl->getUnderlyingType();
      const Type *UT = UnderlyingType.getTypePtr();
      
      // Check if it's a pointer type with address space attribute
      if (!UT->isPointerType()) return;
      
      const PointerType *PT = UT->castAs<PointerType>();
      QualType PointeeType = PT->getPointeeType();
      
      // Look for address space attribute on the pointee type
      const AttributedType *AT = dyn_cast<AttributedType>(PointeeType.getTypePtr());
      if (!AT) return;
      
      // Check if it's a clang::address_space attribute
      bool HasAddressSpace = false;
      if (AT->getAttrKind() == clang::attr::Kind::AddressSpace) {
        HasAddressSpace = true;
      }
      if (!HasAddressSpace)  return;
      
      // Get the address space expression
      const Expr *AddrSpaceExpr = nullptr;
      if (AT->getAttrKind() == clang::attr::Kind::AddressSpace) {
        const auto *AS = dyn_cast<AddressSpaceAttr>(AT->getAttr());
        if (AS) {
          AddrSpaceExpr = AS->getAddressSpace();
        }
      }
      if (!AddrSpaceExpr) return;
      
      // Check if the expression is dependent on template parameters
      if (!AddrSpaceExpr->isValueDependent()) return;
      
      // Get the source range of the address space expression
      SourceRange ExprRange = AddrSpaceExpr->getSourceRange();
      if (ExprRange.isInvalid()) return;
      
      // Get the template parameters of the type alias template
      auto *TemplateParams = TA->getTemplateParameters();
      if (!TemplateParams) return;
      
      // Collect template parameter names for mutation
      templateParamNames.clear();
      for (unsigned i = 0; i < TemplateParams->size(); ++i) {
        auto *Param = TemplateParams->getParam(i);
        if (auto *TTP = dyn_cast<TemplateTypeParmDecl>(Param)) {
          templateParamNames.push_back(TTP->getNameAsString());
        } else if (auto *NTTP = dyn_cast<NonTypeTemplateParmDecl>(Param)) {
          templateParamNames.push_back(NTTP->getNameAsString());
        }
      }
      
      if (templateParamNames.empty()) return;
      
      //Get the source code text of target node
      std::string OriginalExpr = stringutils::rangetoStr(*(Result.SourceManager), ExprRange);
      
      //Perform mutation on the source code text by applying string replacement
      std::string MutatedExpr;
      int MutationChoice = getrandom::getRandomIndex(3);
      
      // Choose a random template parameter name
      size_t ParamIndex = getrandom::getRandomIndex(templateParamNames.size() - 1);
      std::string ParamName = templateParamNames[ParamIndex];
      
      switch (MutationChoice) {
        case 0:
          // Change to param + 1
          MutatedExpr = ParamName + "+1";
          break;
        case 1:
          // Change to param - 1
          MutatedExpr = ParamName + "-1";
          break;
        case 2:
          // Change to 0
          MutatedExpr = "0";
          break;
        case 3:
          // Change to param itself (different param)
          if (templateParamNames.size() > 1) {
            size_t OtherIndex = getrandom::getRandomIndex(templateParamNames.size() - 1);
            while (OtherIndex == ParamIndex) {
              OtherIndex = getrandom::getRandomIndex(templateParamNames.size() - 1);
            }
            MutatedExpr = templateParamNames[OtherIndex];
          } else {
            MutatedExpr = ParamName;
          }
          break;
      }
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(ExprRange, MutatedExpr);
    }
}
  
void MutatorFrontendAction_392::MutatorASTConsumer_392::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher mat2cher = typeAliasTemplateDecl().bind("TypeAliasTemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(mat2cher, &callback);
    matchFinder.matchAST(Context);
}