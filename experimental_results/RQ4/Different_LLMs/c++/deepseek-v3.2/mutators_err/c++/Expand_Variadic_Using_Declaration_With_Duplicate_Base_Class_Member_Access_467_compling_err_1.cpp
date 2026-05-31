//header file
#pragma once
#include "Mutator_base.h"

/**
 * Expand_Variadic_Using_Declaration_With_Duplicate_Base_Class_Member_Access_467
 */ 
class MutatorFrontendAction_467 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(467)

private:
    class MutatorASTConsumer_467 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_467(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::ClassTemplateDecl *> templateDecls;
    };
};

//source file
#include "../include/Mutator_Expand_Variadic_Using_Declaration_With_Duplicate_Base_Class_Member_Access_467.h"

// ========================================================================================================
#define MUT467_OUTPUT 1

void MutatorFrontendAction_467::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::UsingDecl>("UsingDecl")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto usingDeclRange = MT->getSourceRange();
      std::string originalUsingDecl = stringutils::rangetoStr(*(Result.SourceManager), usingDeclRange);
      
      //Check if this using declaration is inside a class template that inherits from a parameter pack
      const clang::DeclContext *context = MT->getDeclContext();
      if (!context) return;
      const clang::CXXRecordDecl *recordDecl = llvm::dyn_cast<clang::CXXRecordDecl>(context);
      if (!recordDecl) return;
      
      //Find the enclosing class template declaration
      const clang::ClassTemplateDecl *templateDecl = nullptr;
      for (const auto &decl : templateDecls) {
        if (decl->getTemplatedDecl() == recordDecl) {
          templateDecl = decl;
          break;
        }
      }
      if (!templateDecl) return;
      
      //Check if the class inherits from a parameter pack
      bool inheritsFromPack = false;
      for (const auto &base : recordDecl->bases()) {
        if (base.getType()->getAs<clang::TemplateTypeParmType>()) {
          inheritsFromPack = true;
          break;
        }
      }
      if (!inheritsFromPack) return;
      
      //Check if the using declaration uses pack expansion
      if (!MT->isPackExpansion()) return;
      
      //Perform mutation on the source code text by applying string replacement
      //First, add the __type_pack_element helper before the class definition if not present
      SourceManager &SM = Rewrite.getSourceMgr();
      SourceLocation templateStart = templateDecl->getBeginLoc();
      
      //Check if __type_pack_element is already defined
      bool typePackElementDefined = false;
      std::string fileContent = stringutils::rangetoStr(SM, CharSourceRange::getCharRange(SM.getLocForStartOfFile(SM.getFileID(templateStart)), templateStart));
      if (fileContent.find("__type_pack_element") != std::string::npos) {
        typePackElementDefined = true;
      }
      
      if (!typePackElementDefined) {
        std::string helperDecl = "template<int, typename...> struct __type_pack_element;\n"
                                 "template<typename T, typename... Rest> struct __type_pack_element<0, T, Rest...> { using type = T; };\n";
        Rewrite.InsertTextBefore(templateStart, helperDecl);
      }
      
      //Add the __first alias inside the class template
      SourceLocation classStart = recordDecl->getBeginLoc();
      std::string firstAlias = "\n    using __first = typename __type_pack_element<0, args...>::type;";
      Rewrite.InsertTextAfterToken(classStart, firstAlias);
      
      //Modify the using declaration to duplicate the member access
      std::string memberName = MT->getNameAsString();
      std::string mutatedUsingDecl = "using __first::" + memberName + " ..., args::" + memberName + " ...;";
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(usingDeclRange), mutatedUsingDecl);
    }
    else if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("ClassTemplateDecl")) {
      //Filter nodes in header files
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;
      //Record the node information to be used in the mutation process
      templateDecls.push_back(TD);
    }
}
  
void MutatorFrontendAction_467::MutatorASTConsumer_467::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher templateMatcher = classTemplateDecl().bind("ClassTemplateDecl");
    DeclarationMatcher usingMatcher = usingDecl(isPackExpansion()).bind("UsingDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(templateMatcher, &callback);
    matchFinder.addMatcher(usingMatcher, &callback);
    matchFinder.matchAST(Context);
}