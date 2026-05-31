//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Nested_Template_Static_Access_454
 */ 
class MutatorFrontendAction_454 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(454)

private:
    class MutatorASTConsumer_454 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_454(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::TypeDecl*> availableTypes;
        std::vector<const clang::VarDecl*> constexprVariables;
    };
};

//source file
#include "../include/MutatorFrontendAction_454.h"

// ========================================================================================================
#define MUT454_OUTPUT 1

void MutatorFrontendAction_454::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::MemberExpr>("StaticMemberAccess")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto memberExpr = MT;
      auto memberDecl = memberExpr->getMemberDecl();
      if (!memberDecl || !isa<clang::VarDecl>(memberDecl) || !dyn_cast<clang::VarDecl>(memberDecl)->isStaticDataMember())
        return;
      
      auto nestedNameSpecifier = memberExpr->getQualifier();
      if (!nestedNameSpecifier)
        return;
      
      auto nestedType = nestedNameSpecifier->getAsType();
      if (!nestedType) {
        auto templateSpecializationType = nestedType->getAs<clang::TemplateSpecializationType>();
        if (!templateSpecializationType)
          return;
        
        auto templateDecl = templateSpecializationType->getTemplateName().getAsTemplateDecl();
        if (!templateDecl)
          return;
        
        //Collect available types and constexpr variables from the seed program
        if (auto *TD = Result.Nodes.getNodeAs<clang::TypeDecl>("TypeDecl")) {
          if (TD && Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation())) {
            availableTypes.push_back(TD);
          }
        }
        if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("ConstexprVar")) {
          if (VD && VD->isConstexpr() && Result.Context->getSourceManager().isWrittenInMainFile(VD->getLocation())) {
            constexprVariables.push_back(VD);
          }
        }
        
        //Perform mutation on the source code text by applying string replacement
        std::string originalText = stringutils::rangetoStr(*Result.SourceManager, MT->getSourceRange());
        
        //Change outer template arguments
        std::string mutatedOuterArgs = "";
        if (availableTypes.size() > 0) {
          size_t index = getrandom::getRandomIndex(availableTypes.size() - 1);
          mutatedOuterArgs = availableTypes[index]->getNameAsString();
        } else {
          mutatedOuterArgs = templateSpecializationType->getTemplateName().getAsTemplateDecl()->getNameAsString();
        }
        
        //Replace constrained NTTP with a different constant expression form
        std::string mutatedNTTP = "";
        if (constexprVariables.size() > 0) {
          size_t index = getrandom::getRandomIndex(constexprVariables.size() - 1);
          mutatedNTTP = constexprVariables[index]->getNameAsString();
        } else {
          //Introduce a new constexpr variable in the nearest enclosing namespace scope
          mutatedNTTP = "zero";
          std::string newConstDecl = "constexpr int zero = 0;\n";
          SourceLocation insertLoc = MT->getBeginLoc();
          Rewrite.InsertTextBefore(insertLoc, newConstDecl);
        }
        
        //Construct mutated static member access expression
        std::string mutatedText = originalText;
        //Replace outer template arguments
        size_t outerStart = mutatedText.find("<");
        size_t outerEnd = mutatedText.find(">", outerStart);
        if (outerStart != std::string::npos && outerEnd != std::string::npos) {
          mutatedText.replace(outerStart + 1, outerEnd - outerStart - 1, mutatedOuterArgs);
        }
        
        //Replace constrained NTTP
        size_t nestedStart = mutatedText.find(">::");
        if (nestedStart != std::string::npos) {
          size_t ntppStart = mutatedText.find(",", nestedStart);
          if (ntppStart != std::string::npos) {
            size_t ntppEnd = mutatedText.find(">", ntppStart);
            if (ntppEnd != std::string::npos) {
              mutatedText.replace(ntppStart + 1, ntppEnd - ntppStart - 1, mutatedNTTP);
            }
          }
        }
        
        mutatedText = "/*mut454*/" + mutatedText;
        
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedText);
      }
    }
}
  
void MutatorFrontendAction_454::MutatorASTConsumer_454::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher staticMemberMatcher = memberExpr(hasQualifier(nestedNameSpecifier(specifiesType(templateSpecializationType()))), hasDecl(isStaticDataMember())).bind("StaticMemberAccess");
    DeclarationMatcher typeMatcher = typeDecl().bind("TypeDecl");
    DeclarationMatcher constexprMatcher = varDecl(isConstexpr()).bind("ConstexprVar");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(staticMemberMatcher, &callback);
    matchFinder.addMatcher(typeMatcher, &callback);
    matchFinder.addMatcher(constexprMatcher, &callback);
    matchFinder.matchAST(Context);
}