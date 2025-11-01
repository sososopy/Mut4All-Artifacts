//source file
#include "../include/Modify_Template_Specialization_NonType_Param_439.h"

// ========================================================================================================
#define MUT439_OUTPUT 1

void MutatorFrontendAction_439::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TS = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("TemplateSpec")) {
        if (!TS || !Result.Context->getSourceManager().isWrittenInMainFile(TS->getLocation()))
            return;

        const TemplateArgument &arg = TS->getTemplateArgs().get(0);
        if (arg.getKind() == TemplateArgument::Integral) {
            llvm::SmallVector<char, 16> buffer;
            arg.getAsIntegral().toString(buffer, 10, true, false);
            std::string originalValue(buffer.begin(), buffer.end());
            std::string mutatedValue = "(" + originalValue + " + 0)";
            
            SourceLocation startLoc = TS->getLocation();
            SourceLocation endLoc = TS->getEndLoc();
            if (startLoc.isValid() && endLoc.isValid()) {
                std::string replacement = "template<> struct " + TS->getNameAsString() + "<" + mutatedValue + "> {";
                Rewrite.ReplaceText(SourceRange(startLoc, endLoc), replacement);
            }
        }
    }
}

void MutatorFrontendAction_439::MutatorASTConsumer_439::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateSpecializationDecl(templateArgumentCountIs(1)).bind("TemplateSpec");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}